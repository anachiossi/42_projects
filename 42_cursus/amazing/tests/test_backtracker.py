"""The backtracker generator: what it promises, checked on what it makes."""

import unittest

from mazegen import Direction, Maze, MazeConfig, MazeGenerator, forty_two
from mazegen.generators.backtracker import Backtracker


def build(width: int = 12, height: int = 8,
          **settings: object) -> Backtracker:
    """A backtracker of that size, seeded so a failure repeats."""
    settings.setdefault("seed", 42)
    config = MazeConfig(
        width, height, (0, 0), (width - 1, height - 1),
        **settings,  # type: ignore[arg-type]
    )
    return Backtracker(config)


def passage_count(maze: Maze) -> int:
    """How many interior walls are open."""
    return sum(1 for (col, row), side in maze.internal_walls()
               if not maze.get_wall(col, row, side))


class RegisteredTest(unittest.TestCase):
    """Finding it by name, which is all the config file can do."""

    def test_it_is_in_the_registry(self) -> None:
        self.assertIs(MazeGenerator.algorithms["backtracker"], Backtracker)

    def test_from_config_builds_one(self) -> None:
        generator = MazeGenerator.from_config(
            MazeConfig(5, 5, (0, 0), (4, 4), gen_algorithm="backtracker"))
        self.assertIsInstance(generator, Backtracker)

    def test_the_name_is_case_and_space_insensitive(self) -> None:
        generator = MazeGenerator.from_config(
            MazeConfig(5, 5, (0, 0), (4, 4),
                       gen_algorithm="  BACKTRACKER "))
        self.assertIsInstance(generator, Backtracker)

    def test_it_is_offered_when_a_name_is_unknown(self) -> None:
        with self.assertRaises(ValueError) as caught:
            MazeGenerator.from_config(
                MazeConfig(5, 5, (0, 0), (4, 4),
                           gen_algorithm="dijkstra"))
        self.assertIn("backtracker", str(caught.exception))


class PerfectMazeTest(unittest.TestCase):
    """PERFECT=True: a spanning tree, and nothing more."""

    def setUp(self) -> None:
        self.width, self.height = 12, 8
        self.maze = build(self.width, self.height, perfect=True).generate()

    def test_every_cell_is_reachable(self) -> None:
        reached = self.maze.region_of(0, 0)
        self.assertEqual(len(reached), self.width * self.height)

    def test_there_are_no_loops(self) -> None:
        """A tree over N cells has exactly N-1 edges; one more is a loop."""
        self.assertEqual(passage_count(self.maze),
                         self.width * self.height - 1)

    def test_the_border_is_untouched(self) -> None:
        for col in range(self.width):
            self.assertTrue(self.maze.get_wall(col, 0, Direction.N))
            self.assertTrue(
                self.maze.get_wall(col, self.height - 1, Direction.S))
        for row in range(self.height):
            self.assertTrue(self.maze.get_wall(0, row, Direction.W))
            self.assertTrue(
                self.maze.get_wall(self.width - 1, row, Direction.E))

    def test_the_ends_are_set(self) -> None:
        self.assertEqual(self.maze.get_entry(), (0, 0))
        self.assertEqual(self.maze.get_exit(),
                         (self.width - 1, self.height - 1))


class BraidedMazeTest(unittest.TestCase):
    """PERFECT=False: loops, no dead-ends, and no single way across."""

    def setUp(self) -> None:
        self.width, self.height = 12, 8
        self.generator = build(self.width, self.height, perfect=False)
        self.maze = self.generator.generate()

    def test_every_cell_is_still_reachable(self) -> None:
        self.assertEqual(len(self.maze.region_of(0, 0)),
                         self.width * self.height)

    def test_there_are_no_dead_ends(self) -> None:
        self.assertEqual(self.maze.dead_ends(), [])

    def test_no_single_passage_cuts_the_exit_off(self) -> None:
        """What "at least two independent routes" comes down to."""
        self.assertEqual(self.generator._choke_points(), [])

    def test_it_has_more_passages_than_a_tree(self) -> None:
        self.assertGreater(passage_count(self.maze),
                           self.width * self.height - 1)


class FortyTwoTest(unittest.TestCase):
    """The shape is sealed before the walk starts, and stays sealed.

    The walk never enters the 42 because those cells begin the run
    already marked as visited. The case that needs saying out loud is
    the cell the walk *starts* from: it is pushed on without being
    asked about, so an entry inside the shape would be carved out of
    unless the wall is checked from that side too.
    """

    def setUp(self) -> None:
        self.width, self.height = 20, 15
        self.glyph = sorted(forty_two.cells(self.width, self.height))

    def maze_from(self, entry: tuple[int, int], seed: int = 1) -> Maze:
        config = MazeConfig(self.width, self.height, entry,
                            (self.width - 1, self.height - 1), seed=seed)
        return Backtracker(config).generate()

    def test_the_shape_is_there_to_begin_with(self) -> None:
        self.assertEqual(len(self.glyph), 26)

    def test_every_cell_of_the_shape_stays_closed(self) -> None:
        maze = self.maze_from((0, 0))
        for cell in maze.get_frozen():
            with self.subTest(cell=cell):
                self.assertEqual(maze.wall_count(*cell), 4)

    def test_an_entry_inside_the_shape_does_not_open_it(self) -> None:
        """The walk starts on a cell of the 42 and must carve nothing."""
        maze = self.maze_from(self.glyph[0])
        opened = [cell for cell in maze.get_frozen()
                  if maze.wall_count(*cell) != 4]
        self.assertEqual(opened, [])

    def test_the_shape_survives_every_entry_of_it(self) -> None:
        for entry in self.glyph:
            with self.subTest(entry=entry):
                maze = self.maze_from(entry)
                self.assertTrue(
                    all(maze.wall_count(*cell) == 4
                        for cell in maze.get_frozen()))

    def test_everything_outside_the_shape_is_reachable(self) -> None:
        maze = self.maze_from((0, 0))
        playable = self.width * self.height - len(maze.get_frozen())
        self.assertEqual(len(maze.region_of(0, 0)), playable)

    def test_no_cell_outside_the_shape_is_walled_in(self) -> None:
        """A cell of four walls that is not the 42 is one nobody drew."""
        maze = self.maze_from((0, 0))
        frozen = maze.get_frozen()
        stranded = [(col, row)
                    for col in range(self.width)
                    for row in range(self.height)
                    if (col, row) not in frozen
                    and maze.wall_count(col, row) == 4]
        self.assertEqual(stranded, [])


class SeedTest(unittest.TestCase):
    """Repeatability, which is the only reason SEED exists."""

    def walls(self, seed: int | None) -> list[bool]:
        maze = build(10, 6, seed=seed).generate()
        return [maze.get_wall(col, row, side)
                for (col, row), side in maze.internal_walls()]

    def test_the_same_seed_gives_the_same_maze(self) -> None:
        self.assertEqual(self.walls(7), self.walls(7))

    def test_a_different_seed_gives_a_different_maze(self) -> None:
        self.assertNotEqual(self.walls(7), self.walls(8))


class CarveTest(unittest.TestCase):
    """The step-by-step form, which the animation will consume."""

    def test_it_yields_more_than_once(self) -> None:
        states = list(build(6, 4, perfect=True).carve())
        self.assertGreater(len(states), 1)

    def test_every_state_is_the_same_live_maze(self) -> None:
        """Documented behaviour: draw as they arrive, do not collect them."""
        states = list(build(6, 4, perfect=True).carve())
        self.assertEqual(len({id(state) for state in states}), 1)

    def test_draining_carve_matches_generate(self) -> None:
        first = build(8, 5, perfect=True, seed=3)
        second = build(8, 5, perfect=True, seed=3)
        for _ in first.carve():
            pass
        expected = second.generate()
        self.assertEqual(first.maze.grid, expected.grid)


class SmallMazeTest(unittest.TestCase):
    """The sizes where a spanning tree barely has room to be one."""

    def test_a_single_cell(self) -> None:
        maze = build(1, 1, perfect=True).generate()
        self.assertEqual(passage_count(maze), 0)
        self.assertEqual(len(maze.region_of(0, 0)), 1)

    def test_a_single_row(self) -> None:
        maze = build(6, 1, perfect=True).generate()
        self.assertEqual(passage_count(maze), 5)
        self.assertEqual(len(maze.region_of(0, 0)), 6)

    def test_a_single_column(self) -> None:
        maze = build(1, 6, perfect=True).generate()
        self.assertEqual(passage_count(maze), 5)
        self.assertEqual(len(maze.region_of(0, 0)), 6)


class SeveralSizesTest(unittest.TestCase):
    """The tree property is not an accident of one seed and one size."""

    def test_perfect_over_a_spread_of_sizes_and_seeds(self) -> None:
        for width, height in ((2, 2), (3, 7), (9, 9), (20, 15)):
            for seed in range(4):
                with self.subTest(size=(width, height), seed=seed):
                    maze = build(width, height,
                                 perfect=True, seed=seed).generate()
                    # The "42" is sealed off, so the tree spans the
                    # cells left over rather than all of them. 20x15
                    # is the only size here with room for the shape.
                    playable = width * height - len(maze.get_frozen())
                    self.assertEqual(len(maze.region_of(0, 0)), playable)
                    self.assertEqual(passage_count(maze), playable - 1)


if __name__ == "__main__":
    unittest.main()
