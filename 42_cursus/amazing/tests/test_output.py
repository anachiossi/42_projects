"""Tests for writing a maze out in the format the subject imposes.

Run from the repository root::

    python3 -m unittest discover -s tests -t .

The format, from IV.5 of the subject:

* one hexadecimal digit a cell, its bits naming the closed walls —
  North is the lowest, then East, South and West;
* cells row by row, one row a line;
* an empty line;
* the entry, the exit and the shortest path, on three lines of their
  own, the path spelt in N, E, S and W;
* every line ends in a newline.

The last case runs the analyzer that ships with the subject over a
written file, because that script is what the defence consults and it
is the only reader here that nobody in this repository wrote.
"""

import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from mazegen import (Direction, Maze, MazeConfig, MazeGenerator,
                     MazeSolver)
from mazegen.maze import ALL_WALLS

ROOT = Path(__file__).resolve().parent.parent
ANALYZER = ROOT / "subject" / "maze_analyzer.py"

SIDES = (Direction.N, Direction.E, Direction.S, Direction.W)


class OutputTest(unittest.TestCase):
    """Common helpers for the cases below."""

    def open_maze(self, width: int, height: int) -> Maze:
        """A maze with every internal wall down, and its border up."""
        maze = Maze(width, height)
        for (col, row), side in maze.internal_walls():
            maze.destroy_wall(col, row, side)
        return maze

    def corridor(self, length: int) -> Maze:
        """A single row of cells, open end to end, entered at the left."""
        maze = self.open_maze(length, 1)
        maze.set_entry((0, 0))
        maze.set_exit((length - 1, 0))
        return maze


class TestLetter(OutputTest):
    """``Direction.letter``: what a step is called in the file."""

    def test_each_side_is_its_own_initial(self) -> None:
        self.assertEqual(Direction.N.letter(), "N")
        self.assertEqual(Direction.E.letter(), "E")
        self.assertEqual(Direction.S.letter(), "S")
        self.assertEqual(Direction.W.letter(), "W")

    def test_every_side_has_one(self) -> None:
        self.assertEqual({side.letter() for side in Direction},
                         set("NESW"))

    def test_it_refuses_several_sides(self) -> None:
        """Read the same way ``opposite`` and ``step`` are."""
        with self.assertRaises(KeyError):
            (Direction.N | Direction.E).letter()


class TestEncode(OutputTest):
    """``Maze.encode``: the grid, as digits."""

    def test_a_closed_maze_is_all_fs(self) -> None:
        maze = Maze(4, 3)
        self.assertEqual(maze.encode(), "\n".join(["ffff"] * 3))

    def test_an_open_maze_keeps_only_its_border(self) -> None:
        """Every digit is what walls the cell still has, border included."""
        maze = self.open_maze(3, 3)
        self.assertEqual(maze.encode(),
                         "9\t1\t3\n8\t0\t2\nc\t4\t6".replace("\t", ""))

    def test_there_are_height_lines_of_width_digits(self) -> None:
        for width, height in ((1, 1), (4, 3), (20, 15), (3, 20)):
            with self.subTest(size=(width, height)):
                lines = Maze(width, height).encode().split("\n")
                self.assertEqual(len(lines), height)
                for line in lines:
                    self.assertEqual(len(line), width)

    def test_the_digits_are_hexadecimal_and_lower_case(self) -> None:
        """The case the subject's own example is written in."""
        maze = self.open_maze(6, 5)
        maze.build_wall(2, 2, ALL_WALLS)
        for char in maze.encode().replace("\n", ""):
            self.assertIn(char, "0123456789abcdef")

    def test_cells_are_stored_row_by_row(self) -> None:
        """A line runs across the grid, which is held in columns."""
        maze = self.open_maze(4, 4)
        maze.build_wall(3, 0, Direction.S)
        lines = maze.encode().split("\n")
        # The top row keeps its northern border, the leftmost cell its
        # western one, and the rightmost its eastern; the wall just
        # built shows up as the S of (3, 0) and the N of (3, 1).
        self.assertEqual(lines[0], "9117")
        self.assertEqual(lines[1], "8003")

    def test_each_digit_is_the_value_the_cell_holds(self) -> None:
        maze = self.open_maze(5, 4)
        maze.build_wall(1, 2, Direction.N | Direction.W)
        lines = maze.encode().split("\n")
        for col in range(maze.width):
            for row in range(maze.height):
                with self.subTest(cell=(col, row)):
                    self.assertEqual(int(lines[row][col], 16),
                                     int(maze.grid[col][row]))


class TestRouteLetters(OutputTest):
    """``Maze.route_letters``: the path, as directions."""

    def test_a_straight_run_east(self) -> None:
        maze = self.corridor(4)
        self.assertEqual(
            maze.route_letters([(0, 0), (1, 0), (2, 0), (3, 0)]), "EEE")

    def test_every_side_is_spelt_correctly(self) -> None:
        """A little loop that leaves each cell by a different side."""
        maze = self.open_maze(2, 2)
        route = [(0, 0), (1, 0), (1, 1), (0, 1), (0, 0)]
        self.assertEqual(maze.route_letters(route), "ESWN")

    def test_a_route_of_one_cell_has_no_steps(self) -> None:
        self.assertEqual(self.corridor(4).route_letters([(0, 0)]), "")

    def test_an_empty_route_is_empty(self) -> None:
        self.assertEqual(self.corridor(4).route_letters([]), "")

    def test_a_jump_to_a_cell_not_next_door_is_refused(self) -> None:
        maze = self.corridor(4)
        with self.assertRaises(ValueError) as raised:
            maze.route_letters([(0, 0), (2, 0)])
        self.assertIn("neighbouring", str(raised.exception))

    def test_a_diagonal_step_is_refused(self) -> None:
        maze = self.open_maze(3, 3)
        with self.assertRaises(ValueError):
            maze.route_letters([(0, 0), (1, 1)])

    def test_standing_still_is_refused(self) -> None:
        maze = self.corridor(4)
        with self.assertRaises(ValueError):
            maze.route_letters([(0, 0), (0, 0)])

    def test_a_step_through_a_standing_wall_is_refused(self) -> None:
        """The one check that makes the file agree with itself."""
        maze = self.corridor(4)
        maze.build_wall(1, 0, Direction.E)
        with self.assertRaises(ValueError) as raised:
            maze.route_letters([(0, 0), (1, 0), (2, 0)])
        self.assertIn("wall", str(raised.exception))

    def test_a_step_off_the_grid_is_refused(self) -> None:
        maze = self.corridor(4)
        with self.assertRaises(ValueError):
            maze.route_letters([(0, 0), (-1, 0)])


class TestAsText(OutputTest):
    """``Maze.as_text``: the whole file, as one string."""

    def text(self) -> str:
        maze = self.corridor(4)
        return maze.as_text([(0, 0), (1, 0), (2, 0), (3, 0)])

    def test_the_whole_file(self) -> None:
        """A row of four open cells, walked from end to end.

        One row high, so every cell keeps its northern and southern
        border — 1 and 4 — and the two ends of the row keep the west
        and the east on top of that.
        """
        self.assertEqual(self.text(), "d557\n\n0,0\n3,0\nEEE\n")

    def test_every_line_ends_in_a_newline(self) -> None:
        """Including the last: the subject asks for it in as many words."""
        self.assertTrue(self.text().endswith("\n"))
        self.assertEqual(self.text().count("\n"),
                         len(self.text().split("\n")) - 1)

    def test_the_grid_comes_first(self) -> None:
        maze = self.corridor(4)
        text = maze.as_text([(0, 0), (1, 0), (2, 0), (3, 0)])
        self.assertTrue(text.startswith(maze.encode() + "\n"))

    def test_an_empty_line_separates_the_grid_from_the_rest(self) -> None:
        maze = self.open_maze(5, 3)
        maze.set_entry((0, 0))
        maze.set_exit((4, 2))
        lines = maze.as_text([]).split("\n")
        self.assertEqual(lines[3], "")

    def test_the_ends_are_written_column_first(self) -> None:
        """``x,y``, the way the configuration file writes them."""
        maze = self.open_maze(6, 4)
        maze.set_entry((1, 3))
        maze.set_exit((5, 2))
        lines = maze.as_text([]).split("\n")
        self.assertEqual(lines[5], "1,3")
        self.assertEqual(lines[6], "5,2")

    def test_the_route_is_the_last_line(self) -> None:
        maze = self.corridor(3)
        lines = maze.as_text([(0, 0), (1, 0), (2, 0)]).split("\n")
        self.assertEqual(lines[-2], "EE")
        self.assertEqual(lines[-1], "")

    def test_a_maze_with_no_ends_is_refused(self) -> None:
        for entry, exit_cell in (((0, 0), None), (None, (3, 0)),
                                 (None, None)):
            with self.subTest(entry=entry, exit=exit_cell):
                maze = self.open_maze(4, 1)
                maze.set_entry(entry)
                maze.set_exit(exit_cell)
                with self.assertRaises(ValueError) as raised:
                    maze.as_text([])
                self.assertIn("entry", str(raised.exception))

    def test_a_route_that_does_not_join_the_ends_is_refused(self) -> None:
        maze = self.corridor(4)
        for route in ([(1, 0), (2, 0), (3, 0)],
                      [(0, 0), (1, 0), (2, 0)],
                      [(1, 0), (2, 0)]):
            with self.subTest(route=route):
                with self.assertRaises(ValueError) as raised:
                    maze.as_text(route)
                self.assertIn("route", str(raised.exception))

    def test_a_maze_with_no_route_still_writes_its_grid(self) -> None:
        """An empty route is a blank line, not a refusal.

        A maze nobody can cross is still a maze worth writing out, and
        the file says so by leaving the path empty.
        """
        text = self.corridor(4).as_text([])
        self.assertTrue(text.endswith("0,0\n3,0\n\n"))


class TestSave(OutputTest):
    """``Maze.save``: the same thing, on disk."""

    def setUp(self) -> None:
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.path = Path(self.folder.name) / "maze.txt"

    def test_the_file_holds_exactly_what_as_text_says(self) -> None:
        maze = self.corridor(5)
        route = [(col, 0) for col in range(5)]
        maze.save(str(self.path), route)
        self.assertEqual(self.path.read_text(encoding="utf-8"),
                         maze.as_text(route))

    def test_writing_twice_replaces_the_file(self) -> None:
        """The program regenerates; the file follows, it does not grow."""
        maze = self.corridor(5)
        route = [(col, 0) for col in range(5)]
        maze.save(str(self.path), route)
        maze.save(str(self.path), route)
        self.assertEqual(self.path.read_text(encoding="utf-8"),
                         maze.as_text(route))

    def test_a_folder_that_is_not_there_comes_back_as_an_oserror(self
                                                                 ) -> None:
        maze = self.corridor(3)
        missing = self.path.parent / "nowhere" / "maze.txt"
        with self.assertRaises(OSError):
            maze.save(str(missing), [(0, 0), (1, 0), (2, 0)])

    def test_nothing_is_written_when_the_route_is_wrong(self) -> None:
        """The file is opened before the text is built, so it would be
        left empty; better an untouched file than a truncated one."""
        maze = self.corridor(4)
        self.path.write_text("previous run\n", encoding="utf-8")
        with self.assertRaises(ValueError):
            maze.save(str(self.path), [(0, 0), (2, 0)])
        self.assertEqual(self.path.read_text(encoding="utf-8"),
                         "previous run\n")


class TestTheAnalyzerReadsIt(OutputTest):
    """A real maze, written out, read by the subject's own script."""

    def setUp(self) -> None:
        self.folder = tempfile.TemporaryDirectory()
        self.addCleanup(self.folder.cleanup)
        self.path = Path(self.folder.name) / "maze.txt"

    def report(self, perfect: bool) -> str:
        config = MazeConfig(20, 15, (0, 0), (19, 14),
                            perfect=perfect, seed=1)
        maze = MazeGenerator.from_config(config).generate()
        route = MazeSolver.from_config(config).solve(maze)
        maze.save(str(self.path), route)
        finished = subprocess.run(
            [sys.executable, str(ANALYZER), str(self.path)],
            capture_output=True, text=True, timeout=120)
        self.assertEqual(finished.returncode, 0, finished.stderr)
        return finished.stdout

    def test_the_walls_are_coherent_and_the_ends_are_found(self) -> None:
        """Incoherent walls are the analyzer's first refusal, and fatal."""
        report = self.report(False)
        self.assertIn("Maze size        : 20 x 15", report)
        self.assertIn("Entry            : (0, 0)", report)
        self.assertIn("Exit: (19, 14) (reachable)", report)
        self.assertIn("Wall coherence   : OK", report)

    def test_a_perfect_maze_is_reported_perfect(self) -> None:
        self.assertIn("PERFECT maze", self.report(True))

    def test_the_default_maze_is_reported_playable(self) -> None:
        self.assertIn("Pac-Man-USABLE", self.report(False))


if __name__ == "__main__":
    unittest.main()
