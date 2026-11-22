"""Tests for the terminal renderer.

Run from the repository root::

    python3 -m unittest discover -s tests -t .

The renderer is checked against the real ``Maze``: ``randomize()``
gives a grid with walls up and down all over, which is exactly what a
drawing has to survive even though it is not a maze anyone could walk.
Everything else is built with ``destroy_wall`` and ``build_wall``, so
the cases stay readable.
"""

import contextlib
import dataclasses
import io
import os
import random
import re
import unittest
from collections.abc import Iterator
from itertools import product
from unittest.mock import patch

from mazegen import Direction, Maze

from app.render.terminal import (CELL_WIDTH, FRAME, STANDARD_SKIN,
                                 AnimationFrame,
                                 Drawing, Element, Frame, JoinedElement,
                                 animate, clear_screen, colour_menu,
                                 drawing_size, maze_junction, maze_wall_above,
                                 maze_wall_left, merge_paths, paint,
                                 path_frames, path_sides,
                                 redraw, render, seen_cells, show,
                                 show_cursor, TooSmallTerminal)

SIDES = (Direction.N, Direction.E, Direction.S, Direction.W)


def plain(drawing: str) -> str:
    """Return a drawing with its escape sequences taken out."""
    return re.sub(r"\033\[[0-9;]*m", "", drawing)


def drawn(maze: Maze, drawing: Drawing | None = None) -> str:
    """Render a maze and strip the colours: the shape on its own.

    A drawing is always coloured now, so every case about *shape* goes
    through here, and the ones about colour call ``render`` directly.
    """
    return plain(render(maze, drawing))


def overwritten(drawing: str) -> str:
    """A drawing as ``redraw`` writes it: each line erasing its row."""
    return drawing.replace("\n", "\033[K\n")


FLUSH = Frame(head=0, tail=0, sides=0)
WALL_SYMBOLS = (set(STANDARD_SKIN.wall.joints
                    + STANDARD_SKIN.border.joints) - {" "})


class TerminalTest(unittest.TestCase):
    """Common helpers for the cases below."""

    def open_maze(self, width: int, height: int) -> Maze:
        """Return a maze whose every internal wall is down."""
        maze = Maze(width, height)
        for col, row in product(range(width), range(height)):
            if col + 1 < width:
                maze.destroy_wall(col, row, Direction.E)
            if row + 1 < height:
                maze.destroy_wall(col, row, Direction.S)
        return maze

    def random_maze(self, width: int, height: int, seed: int = 42) -> Maze:
        """Return the same randomised maze every run."""
        random.seed(seed)
        maze = Maze(width, height)
        maze.randomize()
        return maze

    def lines(self, maze: Maze, drawing: Drawing | None = None) -> list[str]:
        """Render a maze flush in its canvas, as lines of plain text.

        A drawing is always coloured, so a case that is about *shape*
        strips the escapes first. The colour cases below read the
        escapes instead, without stripping.
        """
        plan = drawing or Drawing()
        plan.frame = FLUSH
        return drawn(maze, plan).split("\n")

    def middle(self, col: int, row: int,
               cell_width: int = CELL_WIDTH) -> tuple[int, int]:
        """Return the character in the middle of a cell.

        The renderer works this out as it goes; the tests need it too,
        and writing it twice is the point — a formula that only agrees
        with itself proves nothing.
        """
        return 2 * row + 1, col * (cell_width + 1) + 1 + cell_width // 2

    def side(self, col: int, row: int, direction: Direction,
             cell_width: int = CELL_WIDTH) -> tuple[int, int]:
        """Return the character in the middle of one side of a cell."""
        char_row, char_col = self.middle(col, row, cell_width)
        if direction is Direction.N:
            return char_row - 1, char_col
        if direction is Direction.S:
            return char_row + 1, char_col
        anchor = col * (cell_width + 1)
        if direction is Direction.W:
            return char_row, anchor
        return char_row, anchor + cell_width + 1


class SegmentTest(TerminalTest):
    """Reading a wall from the two cells that share it."""

    def test_inside_the_maze(self) -> None:
        """A segment is the wall the two cells agree on."""
        maze = Maze(3, 2)
        maze.destroy_wall(0, 0, Direction.E)
        self.assertFalse(maze_wall_left(maze, 1, 0))
        self.assertTrue(maze_wall_left(maze, 2, 0))

    def test_the_last_row_and_column(self) -> None:
        """Past the last cell, the segment is read from the other side."""
        maze = Maze(2, 2)
        self.assertTrue(maze_wall_left(maze, 2, 0))     # E of the last column
        self.assertTrue(maze_wall_above(maze, 0, 2))   # S of the last row

    def test_a_segment_that_does_not_exist(self) -> None:
        """No cell shares it, so there is no wall — not a wall by default.

        This is what draws the corners: at a corner of the frame two of
        the four arms are simply not there.
        """
        maze = Maze(2, 2)
        self.assertFalse(maze_wall_left(maze, 0, -1))
        self.assertFalse(maze_wall_above(maze, -1, 0))
        self.assertFalse(maze_wall_above(maze, 2, 0))


class WallTest(TerminalTest):
    """The walls between the cells."""

    def test_closed_cell_is_a_closed_box(self) -> None:
        """A 1x1 maze is a box in the border's own symbols."""
        self.assertEqual(self.lines(Maze(1, 1)),
                         ["┏━━━┓", "┃   ┃", "┗━━━┛"])

    def test_open_wall_becomes_floor(self) -> None:
        """Opening a wall leaves nothing where it was."""
        maze = Maze(2, 1)
        self.assertEqual(self.lines(maze)[1], "┃   │   ┃")
        maze.destroy_wall(0, 0, Direction.E)
        self.assertEqual(self.lines(maze)[1], "┃       ┃")

    def test_open_maze_has_no_inside_walls(self) -> None:
        """A board with every inside wall down draws empty."""
        for line in self.lines(self.open_maze(4, 3))[1:-1]:
            self.assertEqual(line[1:-1].strip(), "")

    def test_every_wall_of_a_random_maze_is_drawn(self) -> None:
        """What the drawing shows is what ``get_wall`` says.

        The renderer reads each wall once, from one of the two cells
        that share it; this checks all four sides of every cell, which
        is where reading it once could go wrong.
        """
        maze = self.random_maze(7, 5)
        lines = self.lines(maze)
        for col, row in product(range(7), range(5)):
            for direction in SIDES:
                char_row, char_col = self.side(col, row, direction)
                with self.subTest(col=col, row=row, side=direction.name):
                    self.assertEqual(
                        lines[char_row][char_col] in WALL_SYMBOLS,
                        maze.get_wall(col, row, direction),
                    )


class BorderTest(TerminalTest):
    """The outer wall, in its own skin."""

    def test_border_is_heavier_than_the_walls(self) -> None:
        """The frame is drawn with its own element, not the wall's."""
        lines = self.lines(self.open_maze(3, 2))
        self.assertEqual(lines[0], "┏" + "━" * 11 + "┓")
        self.assertEqual(lines[-1], "┗" + "━" * 11 + "┛")
        for line in lines[1:-1]:
            self.assertTrue(line.startswith("┃"))
            self.assertTrue(line.endswith("┃"))

    def test_the_border_is_drawn_closed_whatever_the_maze_says(
            self) -> None:
        """The border is drawn first, and never asked about.

        The outside of a maze is walled by definition — the subject
        requires it and ``Maze`` refuses to open it — so the four edges
        are drawn without reading the grid. That is what leaves the
        north-and-west rule with no exception inside.

        The price, written down here so it is a decision and not a
        surprise: a border broken by hand does not show. It can only
        happen by reaching past ``Maze``'s own API, as this case does.
        """
        broken = Maze(3, 2)
        broken.grid[1][0] &= ~Direction.N
        lines = self.lines(broken)
        self.assertEqual(lines[0], self.lines(Maze(3, 2))[0])
        self.assertNotIn(" ", lines[0])
        self.assertNotIn(" ", lines[-1])


class JunctionTest(TerminalTest):
    """Where walls meet."""

    def test_corners_of_the_frame(self) -> None:
        """Each corner of the border gets its own heavy symbol."""
        lines = self.lines(Maze(2, 2))
        self.assertEqual((lines[0][0], lines[0][-1]), ("┏", "┓"))
        self.assertEqual((lines[-1][0], lines[-1][-1]), ("┗", "┛"))

    def test_a_wall_running_into_the_frame(self) -> None:
        """The frame keeps its weight where a thin wall meets it."""
        lines = self.lines(Maze(2, 2))
        self.assertIn("┯", lines[0])
        self.assertIn("┷", lines[-1])
        self.assertIn("┠", [line[0] for line in lines])
        self.assertIn("┨", [line[-1] for line in lines])

    def test_a_junction_no_wall_reaches_is_blank(self) -> None:
        """An open floor keeps its colour, with nothing drawn on it."""
        drawing = drawn(self.open_maze(3, 3), Drawing(frame=FLUSH))
        for symbol in ("┼", "┌", "┐", "└", "┘", "├", "┤", "┬", "┴"):
            self.assertNotIn(symbol, drawing)

    def test_a_crossing_inside_the_maze(self) -> None:
        """Four walls meeting inside give the thin crossing."""
        self.assertEqual(
            maze_junction(Maze(3, 3), 1, 1, STANDARD_SKIN)[0], "┼")

    def test_the_element_says_which_table_was_used(self) -> None:
        """A junction on the frame is the border's, inside is the wall's."""
        maze = Maze(3, 3)
        self.assertIs(maze_junction(maze, 0, 0, STANDARD_SKIN)[1],
                      STANDARD_SKIN.border)
        self.assertIs(maze_junction(maze, 1, 1, STANDARD_SKIN)[1],
                      STANDARD_SKIN.wall)


class PathsTest(TerminalTest):
    """More than one path at a time: two searches meeting."""

    def two_ways(self, exit: tuple[int, int] | None = None) -> Maze:
        """A corridor five cells long, open all the way."""
        maze = Maze(5, 1, exit=exit)
        for col in range(4):
            maze.destroy_wall(col, 0, Direction.E)
        return maze

    def test_each_path_keeps_its_own_colour(self) -> None:
        """The skin has an element per path, and they differ."""
        first, second = STANDARD_SKIN.paths[0], STANDARD_SKIN.paths[1]
        self.assertNotEqual(first.ink, second.ink)
        drawing = render(self.two_ways(),
                         Drawing(paths=[[(0, 0), (1, 0)],
                                        [(4, 0), (3, 0)]], frame=FLUSH))
        self.assertIn(first.ink, drawing)
        self.assertIn(second.ink, drawing)

    def test_meeting_turns_every_path_at_once(self) -> None:
        """Sharing one cell makes the paths one route, all of it.

        Two searches growing from the ends of a maze are two colours
        right up to the step where they touch, and one colour from
        that step on — which is what says the maze is solved.
        """
        shared = (2, 0)
        drawing = render(self.two_ways(),
                         Drawing(paths=[[(0, 0), (1, 0), shared],
                                        [(4, 0), (3, 0), shared]],
                                 frame=FLUSH))
        assert STANDARD_SKIN.meeting is not None
        self.assertIn(f"\033[{STANDARD_SKIN.meeting.ink}m", drawing)
        for element in STANDARD_SKIN.paths:
            self.assertNotIn(f"\033[{element.ink}m", drawing)

    def test_reaching_the_exit_turns_the_route_too(self) -> None:
        """One search that gets there on its own is just as solved.

        Nothing was shared, so nothing met — but the route ends at the
        exit, which says the same thing.
        """
        drawing = render(self.two_ways(exit=(4, 0)),
                         Drawing(frame=FLUSH,
                                 paths=[[(0, 0), (1, 0), (2, 0), (3, 0),
                                         (4, 0)]]))
        assert STANDARD_SKIN.meeting is not None
        self.assertIn(f"\033[{STANDARD_SKIN.meeting.ink}m", drawing)
        self.assertNotIn(f"\033[{STANDARD_SKIN.paths[0].ink}m", drawing)

    def test_short_of_the_exit_it_is_still_searching(self) -> None:
        """One cell short is not solved, and does not look it."""
        drawing = render(self.two_ways(exit=(4, 0)),
                         Drawing(frame=FLUSH,
                                 paths=[[(0, 0), (1, 0), (2, 0)]]))
        assert STANDARD_SKIN.meeting is not None
        self.assertNotIn(f"\033[{STANDARD_SKIN.meeting.ink}m", drawing)
        self.assertIn(f"\033[{STANDARD_SKIN.paths[0].ink}m", drawing)

    def test_until_they_meet_each_keeps_its_colour(self) -> None:
        """One step short of touching, they are still two searches."""
        drawing = render(self.two_ways(),
                         Drawing(paths=[[(0, 0), (1, 0)],
                                        [(4, 0), (3, 0)]], frame=FLUSH))
        assert STANDARD_SKIN.meeting is not None
        self.assertNotIn(f"\033[{STANDARD_SKIN.meeting.ink}m", drawing)
        for element in STANDARD_SKIN.paths:
            self.assertIn(f"\033[{element.ink}m", drawing)

    def test_the_solved_route_is_all_one_colour(self) -> None:
        """Handing the finished route in as a path turns all of it.

        Every cell of it is then shared with one half or the other, so
        the whole route reads as the meeting colour — which is what a
        solved maze should look like.
        """
        left, right = [(0, 0), (1, 0), (2, 0)], [(4, 0), (3, 0), (2, 0)]
        solved = left + right[::-1]
        drawing = render(self.two_ways(),
                         Drawing(paths=[left, right, solved], frame=FLUSH))
        assert STANDARD_SKIN.meeting is not None
        for element in STANDARD_SKIN.paths:
            self.assertNotIn(f"\033[{element.ink}m", drawing)
        self.assertIn(f"\033[{STANDARD_SKIN.meeting.ink}m", drawing)

    def test_more_paths_than_colours_start_again(self) -> None:
        """A third path takes the first colour back."""
        elements = STANDARD_SKIN.paths
        self.assertIs(merge_paths([[(0, 0)], [(1, 0)], [(2, 0)]],
                                  elements)[(2, 0)][1], elements[0])

    def test_a_cell_walked_once_keeps_its_path_colour(self) -> None:
        """Only sharing makes a meeting: one path is still itself."""
        elements = STANDARD_SKIN.paths
        links = merge_paths([[(0, 0), (1, 0)]], elements,
                            STANDARD_SKIN.meeting)
        self.assertIs(links[(0, 0)][1], elements[0])


class PathTest(TerminalTest):
    """A continuous line, not a trail of dots."""

    def straight_maze(self) -> Maze:
        """A maze with the top row opened all the way across."""
        maze = Maze(4, 2)
        for col in range(3):
            maze.destroy_wall(col, 0, Direction.E)
        return maze

    def test_sides_of_each_cell(self) -> None:
        """A cell of the path reaches towards its neighbours in it."""
        sides = path_sides([(0, 0), (1, 0), (1, 1)])
        self.assertEqual(sides[(0, 0)], Direction.E)
        self.assertEqual(sides[(1, 0)], Direction.W | Direction.S)
        self.assertEqual(sides[(1, 1)], Direction.N)

    def test_one_cell_is_a_dot(self) -> None:
        """A path of one cell reaches nowhere, so it is a point."""
        lines = self.lines(Maze(2, 1), Drawing(paths=[[(0, 0)]]))
        char_row, char_col = self.middle(0, 0)
        self.assertEqual(lines[char_row][char_col], "•")

    def test_a_straight_path_has_no_holes(self) -> None:
        """The line runs through the cells and the openings between."""
        maze = self.straight_maze()
        lines = self.lines(maze, Drawing(paths=[[(col, 0)
                                                 for col in range(4)]]))
        char_row, first = self.middle(0, 0)
        _, last = self.middle(3, 0)
        drawn = lines[char_row][first:last + 1]
        self.assertNotIn(" ", drawn)
        self.assertEqual(drawn[1:-1], "─" * (len(drawn) - 2))

    def test_a_turn_uses_a_corner(self) -> None:
        """Where the path turns, it draws the corner of the turn."""
        maze = Maze(2, 2)
        maze.destroy_wall(0, 0, Direction.E)
        maze.destroy_wall(1, 0, Direction.S)
        lines = self.lines(maze, Drawing(paths=[[(0, 0), (1, 0), (1, 1)]]))
        char_row, char_col = self.middle(1, 0)
        self.assertEqual(lines[char_row][char_col], "╮")

    def test_the_path_crosses_a_side_wall(self) -> None:
        """Two cells side by side are joined through the opening."""
        maze = Maze(2, 1)
        maze.destroy_wall(0, 0, Direction.E)
        lines = self.lines(maze, Drawing(paths=[[(0, 0), (1, 0)]]))
        opening_row, opening_col = self.side(0, 0, Direction.E)
        self.assertEqual(lines[opening_row][opening_col], "─")

    def test_the_path_crosses_a_wall_above(self) -> None:
        """Two cells one above the other are joined the same way."""
        maze = Maze(1, 2)
        maze.destroy_wall(0, 0, Direction.S)
        lines = self.lines(maze, Drawing(paths=[[(0, 0), (0, 1)]]))
        opening_row, opening_col = self.side(0, 0, Direction.S)
        self.assertEqual(lines[opening_row][opening_col], "│")

    def test_ends_are_drawn_over_the_path(self) -> None:
        """Entry and exit stay readable on a solved maze."""
        maze = self.straight_maze()
        maze.set_entry((0, 0))
        maze.set_exit((3, 0))
        lines = self.lines(maze, Drawing(paths=[[(col, 0)
                                                 for col in range(4)]]))
        for cell, symbol in (((0, 0), STANDARD_SKIN.entry.symbol),
                             ((3, 0), STANDARD_SKIN.exit.symbol)):
            char_row, char_col = self.middle(*cell)
            self.assertEqual(lines[char_row][char_col], symbol)


class MarkTest(TerminalTest):
    """One symbol in the middle of a cell."""

    def test_entry_and_exit(self) -> None:
        """Both ends are drawn in the middle of their own cell."""
        lines = self.lines(Maze(3, 2, entry=(0, 0), exit=(2, 1)))
        for cell, symbol in (((0, 0), STANDARD_SKIN.entry.symbol),
                             ((2, 1), STANDARD_SKIN.exit.symbol)):
            char_row, char_col = self.middle(*cell)
            self.assertEqual(lines[char_row][char_col], symbol)

    def test_the_two_characters(self) -> None:
        """Char1 and Char2 are marks like any other."""
        lines = self.lines(Maze(3, 2), Drawing(char1=(1, 0), char2=(2, 0)))
        assert STANDARD_SKIN.char1 is not None
        assert STANDARD_SKIN.char2 is not None
        for cell, element in (((1, 0), STANDARD_SKIN.char1),
                              ((2, 0), STANDARD_SKIN.char2)):
            char_row, char_col = self.middle(*cell)
            self.assertEqual(lines[char_row][char_col], element.symbol)

    def test_an_end_outside_the_maze_is_refused(self) -> None:
        """A cell that does not exist cannot be an end at all.

        It used to be dropped quietly at drawing time. Now that the
        ends belong to the maze, the mistake is caught where it is
        made instead of going missing from the picture.
        """
        for cell in ((9, 9), (-1, 0)):
            with self.subTest(cell=cell):
                with self.assertRaises(IndexError):
                    Maze(2, 2, entry=cell)
                with self.assertRaises(IndexError):
                    Maze(2, 2, exit=cell)


class VisitedTest(TerminalTest):
    """Where the search went, under everything else in the cell."""

    def dot(self, front: int = 0) -> str:
        """The symbol a cell of that search front is drawn with."""
        return STANDARD_SKIN.seen[front].symbol

    def test_a_visited_cell_is_marked(self) -> None:
        """A cell the search went through is no longer bare floor."""
        lines = self.lines(Maze(3, 2), Drawing(visited=[[(1, 0), (2, 1)]]))
        for col, row in ((1, 0), (2, 1)):
            char_row, char_col = self.middle(col, row)
            self.assertEqual(lines[char_row][char_col], self.dot())

    def test_a_cell_left_alone_stays_floor(self) -> None:
        """Only the cells given are marked, and nothing else is."""
        lines = self.lines(Maze(3, 2), Drawing(visited=[[(1, 0)]]))
        char_row, char_col = self.middle(0, 0)
        self.assertEqual(lines[char_row][char_col],
                         STANDARD_SKIN.floor.symbol)

    def test_a_drawing_with_no_search_has_no_dots(self) -> None:
        """A drawing that was never solved looks as it always did."""
        drawing = "\n".join(self.lines(self.random_maze(6, 4)))
        self.assertNotIn(self.dot(), drawing)

    def test_the_path_is_drawn_over_its_own_search(self) -> None:
        """Every cell of a route was searched first, so the route wins.

        The other way round the search would rub out the answer it
        was looking for.
        """
        maze = self.open_maze(3, 1)
        route = [(0, 0), (1, 0), (2, 0)]
        lines = self.lines(maze, Drawing(paths=[route], visited=[route]))
        char_row, char_col = self.middle(1, 0)
        self.assertNotEqual(lines[char_row][char_col], self.dot())
        self.assertIn(lines[char_row][char_col],
                      STANDARD_SKIN.paths[0].joints)

    def test_the_ends_are_drawn_over_the_search(self) -> None:
        """A mark beats the search, the same way it beats the path."""
        maze = Maze(3, 2, entry=(0, 0), exit=(2, 1))
        lines = self.lines(maze, Drawing(visited=[[(0, 0), (2, 1)]]))
        for cell, symbol in (((0, 0), STANDARD_SKIN.entry.symbol),
                             ((2, 1), STANDARD_SKIN.exit.symbol)):
            char_row, char_col = self.middle(cell[0], cell[1])
            self.assertEqual(lines[char_row][char_col], symbol)

    def test_each_front_is_drawn_in_its_own_element(self) -> None:
        """Two searches from two ends are told apart, like two routes."""
        skin = dataclasses.replace(
            STANDARD_SKIN, seen=(Element("a"), Element("b")))
        lines = self.lines(Maze(3, 1),
                           Drawing(visited=[[(0, 0)], [(2, 0)]], skin=skin))
        for cell, symbol in ((((0, 0)), "a"), (((2, 0)), "b")):
            char_row, char_col = self.middle(cell[0], cell[1])
            self.assertEqual(lines[char_row][char_col], symbol)

    def test_a_cell_reached_by_both_keeps_the_first(self) -> None:
        """Where two fronts meet, the one that got there first shows."""
        skin = dataclasses.replace(
            STANDARD_SKIN, seen=(Element("a"), Element("b")))
        seen = seen_cells(Drawing(visited=[[(1, 0)], [(1, 0)]]), skin)
        self.assertEqual(seen[(1, 0)].symbol, "a")

    def test_a_skin_with_no_element_draws_nothing(self) -> None:
        """A skin that says nothing about the search shows none of it."""
        skin = dataclasses.replace(STANDARD_SKIN, seen=())
        self.assertEqual(seen_cells(Drawing(visited=[[(1, 0)]]), skin), {})


class FrameTest(TerminalTest):
    """The canvas around the maze, and the text in it."""

    def test_the_canvas_is_bigger_than_the_maze(self) -> None:
        """Head, tail and sides are added to the maze's own size."""
        frame = Frame(head=2, tail=3, sides=2)
        lines = drawn(Maze(4, 3), Drawing(frame=frame)).split("\n")
        self.assertEqual(len(lines), 2 * 3 + 1 + frame.head + frame.tail)
        self.assertEqual(len(lines[0]),
                         4 * (CELL_WIDTH + 1) + 1 + 2 * frame.sides)

    def test_the_title_is_centred(self) -> None:
        """The head carries the title, in the middle of the picture."""
        frame = Frame(head=2, tail=2, sides=2)
        lines = drawn(Maze(6, 3), Drawing(title="MAZE",
                                          frame=frame)).split("\n")
        line = next(line for line in lines if "MAZE" in line)
        left = line.index("MAZE")
        self.assertLessEqual(abs(left - (len(line) - left - 4)), 1)

    def test_the_tail_is_centred_too(self) -> None:
        """A message sits in the middle, like the title."""
        frame = Frame(head=1, tail=2, sides=2)
        lines = drawn(Maze(6, 3), Drawing(message=["ready"],
                                          frame=frame)).split("\n")
        line = next(line for line in lines if "ready" in line)
        left = line.index("ready")
        self.assertLessEqual(abs(left - (len(line) - left - 5)), 1)

    def test_the_colour_palette_is_centred(self) -> None:
        """The C-key swatches sit on the same canvas as the message."""
        maze = Maze(20, 15)
        drawing = Drawing()
        line = colour_menu(maze, drawing)
        visible = plain(line)
        columns, _ = drawing_size(20, 15, drawing.cell_width, drawing.frame)
        self.assertEqual(len(visible), columns)
        stripped = visible.strip()
        left = visible.index(stripped[0])
        right = len(visible) - visible.rindex(stripped[-1]) - 1
        self.assertLessEqual(abs(left - right), 1)

    def test_the_tail_is_reserved_when_empty(self) -> None:
        """The maze does not move when there is nothing to say."""
        frame = Frame(head=1, tail=3, sides=1)
        quiet = drawn(Maze(4, 3), Drawing(frame=frame)).split("\n")
        talking = drawn(Maze(4, 3), Drawing(message=["hello"],
                                            frame=frame)).split("\n")
        self.assertEqual(len(quiet), len(talking))
        self.assertEqual(quiet[:-3], talking[:-3])

    def test_a_long_message_is_wrapped(self) -> None:
        """The tail grows to hold it, and every character survives."""
        frame = Frame(head=1, tail=2, sides=1)
        lines = drawn(Maze(3, 2), Drawing(message=["x" * 200],
                                          frame=frame)).split("\n")
        self.assertEqual({len(line) for line in lines}, {len(lines[0])})
        self.assertEqual("".join(lines).count("x"), 200)

    def test_more_lines_than_the_tail_holds(self) -> None:
        """A message past the tail makes the tail taller, not shorter."""
        frame = Frame(head=0, tail=1, sides=0)
        lines = drawn(Maze(3, 2), Drawing(message=["one", "two"],
                                          frame=frame)).split("\n")
        self.assertIn("one", lines[-2])
        self.assertIn("two", lines[-1])

    def test_a_message_that_fits_is_left_alone(self) -> None:
        """Spacing put there on purpose is not spent unless it has to be."""
        frame = Frame(head=0, tail=1, sides=0)
        spaced = "R  redraw   Q  quit"
        lines = drawn(Maze(12, 2), Drawing(message=[spaced],
                                           frame=frame)).split("\n")
        self.assertIn(spaced, lines[-1])


class ColourTest(TerminalTest):
    """The colours, added as each run of characters is written."""

    def test_the_drawing_is_always_coloured(self) -> None:
        """There is no plain mode: escapes are always written."""
        self.assertIn("\033[", render(self.random_maze(5, 4)))

    def test_stripping_the_escapes_leaves_the_rectangle(self) -> None:
        """What the colours are wrapped around is still a rectangle."""
        drawing = drawn(self.random_maze(5, 4),
                        Drawing(title="T", message=["m"]))
        widths = {len(line) for line in drawing.split("\n")}
        self.assertEqual(len(widths), 1)

    def test_a_run_is_wrapped_once(self) -> None:
        """Characters sharing a style share one escape sequence."""
        self.assertEqual(paint("───", STANDARD_SKIN.wall),
                         f"\033[{STANDARD_SKIN.wall.ink}m───\033[0m")

    def test_nothing_is_drawn_on_a_background(self) -> None:
        """Every colour in a drawing is ink: nothing sets a background.

        One character holds one thing, so a symbol is how anything is
        said — including the "42", which is a block and not a dye.
        """
        drawing = render(self.random_maze(4, 3),
                         Drawing(title="T", message=["m"]))
        self.assertNotIn("48;5;", drawing)

    def test_no_style_no_escape(self) -> None:
        """A colourless element is written as it is."""
        self.assertEqual(paint("ab", Element("a")), "ab")


class SkinTest(TerminalTest):
    """The elements, and the skin that holds them."""

    def test_joints_needs_sixteen_symbols(self) -> None:
        """A joints table that cannot answer every junction is refused."""
        with self.assertRaises(ValueError):
            JoinedElement(joints="+" * 15)

    def test_a_piece_is_looked_up_by_its_sides(self) -> None:
        """The table is indexed by the Direction bits, nothing else."""
        wall = STANDARD_SKIN.wall
        self.assertEqual(wall.piece(Direction.E | Direction.W), "─")
        self.assertEqual(wall.piece(Direction.N | Direction.S), "│")
        self.assertEqual(STANDARD_SKIN.wall.piece(0), " ")

    def test_straight_runs_the_way_it_is_asked(self) -> None:
        """A wall running east is the same piece as one running west."""
        self.assertEqual(STANDARD_SKIN.wall.straight(Direction.E),
                         STANDARD_SKIN.wall.straight(Direction.W))
        self.assertEqual(STANDARD_SKIN.wall.straight(Direction.N),
                         STANDARD_SKIN.wall.straight(Direction.S))

    def test_a_skin_reaches_every_symbol_of_the_drawing(self) -> None:
        """Nothing is drawn with a character the skin did not give.

        This is what "a new look is a new instance and not new code"
        has to mean: a symbol hard-coded anywhere in the drawing would
        survive the change of skin and show up here.
        """
        maze = self.random_maze(5, 4)
        maze.set_entry((0, 0))
        maze.set_exit((4, 3))
        drawing = drawn(maze, Drawing(paths=[[(1, 1), (2, 1)]], title="T",
                                      message=["m"]))
        skin = STANDARD_SKIN
        allowed = set(skin.wall.joints + skin.border.joints
                      + skin.paths[0].joints + skin.paths[1].joints
                      + skin.floor.symbol + skin.entry.symbol
                      + skin.exit.symbol + "Tm\n")
        self.assertEqual(set(drawing) - allowed, set())


class AnimationTest(TerminalTest):
    """The same drawing, again."""

    def capture(self, call: object) -> str:
        """Run something and return what it printed."""
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            call()  # type: ignore[operator]
        return out.getvalue()

    def test_show_prints_the_drawing_in_colour(self) -> None:
        """``show`` is the coloured drawing, printed."""
        maze = self.random_maze(4, 3)
        self.assertEqual(self.capture(lambda: show(maze)),
                         render(maze) + "\n")

    def test_clear_screen_wipes_the_screen(self) -> None:
        """A real wipe, used once before the first frame."""
        self.assertEqual(self.capture(clear_screen), "\033[H\033[J")

    def test_a_frame_overwrites_instead_of_wiping(self) -> None:
        """No blank screen between frames, and no trailing newline.

        Wiping first leaves the screen empty until the frame arrives,
        and a trailing newline scrolls a window the drawing exactly
        fills. Both of those are what a flicker is made of.
        """
        maze = self.random_maze(4, 3)
        written = self.capture(lambda: redraw(maze))
        self.assertTrue(written.startswith("\033[H"))
        self.assertNotIn("\033[J", written[:-3])
        self.assertTrue(written.endswith("\033[K\033[J"))
        self.assertFalse(written.endswith("\n"))

    def test_every_line_erases_the_rest_of_its_row(self) -> None:
        """Each row is cleared as it is overwritten, not beforehand."""
        maze = self.random_maze(4, 3)
        written = self.capture(lambda: redraw(maze))
        self.assertEqual(written.count("\033[K"),
                         len(render(maze).split("\n")))

    def test_a_maze_too_big_for_the_window_is_refused(self) -> None:
        """No drawing at all, and a message the reader can act on."""
        with patch("shutil.get_terminal_size",
                   return_value=os.terminal_size((40, 12))):
            with self.assertRaises(TooSmallTerminal) as refused:
                render(Maze(20, 15))
        said = str(refused.exception)
        self.assertIn("85 columns x 36 lines", said)
        self.assertIn("40 x 12", said)
        self.assertIn("45 columns", said)
        self.assertIn("24 lines", said)
        self.assertIn("ctrl and minus", said)

    def test_a_window_that_fits_exactly_is_not_refused(self) -> None:
        """Needing all of it is still fitting in it."""
        needed = drawing_size(20, 15, CELL_WIDTH, FRAME)
        with patch("shutil.get_terminal_size",
                   return_value=os.terminal_size(needed)):
            self.assertTrue(render(Maze(20, 15), Drawing(frame=FRAME)))

    def test_the_cursor_is_hidden_while_animating(self) -> None:
        """And put back afterwards, so the terminal is left usable."""
        printed = self.capture(lambda: animate([Maze(2, 2)], delay=0.0))
        self.assertIn("\033[?25l", printed)
        self.assertTrue(printed.endswith("\033[?25h"))

    def test_the_cursor_comes_back_even_when_animating_fails(self) -> None:
        """A ctrl-c mid-animation must not leave the cursor gone."""
        def boom() -> None:
            def frames() -> Iterator[Maze]:
                yield Maze(2, 2)
                raise KeyboardInterrupt
            animate(frames(), delay=0.0)
        with self.assertRaises(KeyboardInterrupt):
            self.capture(boom)
        self.assertEqual(self.capture(show_cursor), "\033[?25h")

    def test_one_frame_per_state(self) -> None:
        """Each state given is drawn once, in the order given."""
        mazes = [Maze(2, 2), self.random_maze(2, 2), self.open_maze(2, 2)]
        printed = self.capture(lambda: animate(mazes, delay=0.0))
        self.assertEqual(printed.count("\033[H\033[J"), 1)
        self.assertEqual(printed.count("\033[H"), len(mazes) + 1)
        for maze in mazes:
            self.assertIn(overwritten(render(maze)), printed)

    def test_one_frame_in_every_so_many_is_drawn(self) -> None:
        """A long carve is made watchable by showing less of it.

        Drawing a frame costs far more than making one, so the frames
        that are skipped still happen — they are simply not drawn.
        """
        mazes = [self.random_maze(2, 2, seed) for seed in range(10)]
        printed = self.capture(lambda: animate(mazes, delay=0.0, every=3))
        # Frames 0, 3, 6 and 9, plus the one wipe before the first.
        self.assertEqual(printed.count("\033[H"), 4 + 1)

    def test_the_last_frame_is_drawn_whatever_the_count(self) -> None:
        """What is left on the screen has to be the finished picture."""
        mazes = [self.random_maze(2, 2, seed) for seed in range(9)]
        printed = self.capture(lambda: animate(mazes, delay=0.0, every=3))
        # Frames 0, 3 and 6 by the count, then 8 because it is last.
        self.assertEqual(printed.count("\033[H"), 4 + 1)
        self.assertIn(overwritten(render(mazes[-1])), printed)

    def test_one_in_every_one_is_every_frame(self) -> None:
        """The default counts nothing out."""
        mazes = [self.random_maze(2, 2, seed) for seed in range(4)]
        printed = self.capture(lambda: animate(mazes, delay=0.0, every=1))
        self.assertEqual(printed.count("\033[H"), len(mazes) + 1)

    def test_a_frame_in_fewer_than_one_is_refused(self) -> None:
        """Nothing is drawn at all, rather than every frame twice."""
        with self.assertRaises(ValueError):
            animate([Maze(2, 2)], delay=0.0, every=0)

    def test_a_frame_can_carry_its_own_drawing(self) -> None:
        """A snapshot is a maze and what to draw on it, together."""
        entered = self.open_maze(3, 2)
        entered.set_entry((0, 0))
        left = self.open_maze(3, 2)
        left.set_exit((2, 1))
        frames = [AnimationFrame(entered, Drawing(title="in")),
                  AnimationFrame(left, Drawing(title="out"))]
        printed = self.capture(lambda: animate(frames, delay=0.0))
        self.assertIn(overwritten(render(entered, Drawing(title="in"))),
                      printed)
        self.assertIn(overwritten(render(left, Drawing(title="out"))),
                      printed)

    def test_a_path_is_revealed_one_cell_at_a_time(self) -> None:
        """The frames of a path grow by a cell each time.

        That is what an animated solver is: the route arriving, not the
        route arrived.
        """
        maze = self.open_maze(4, 1)
        path = [(0, 0), (1, 0), (2, 0), (3, 0)]
        frames = list(path_frames(maze, Drawing(paths=[path])))
        self.assertEqual(len(frames), len(path) + 1)
        lengths = [len(frame.drawing.paths[0]) for frame in frames]
        self.assertEqual(lengths, [0, 1, 2, 3, 4])


if __name__ == "__main__":
    unittest.main()
