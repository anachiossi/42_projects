"""Bidirectional A*: checked against BFS, which is the ground truth.

``solvers/__init__`` sets the bar for a second solver: the same route
length as BFS, on every maze, for every seed. That is what
``GroundTruthTest`` walks, over both generators, perfect and braided,
at three sizes. The rest of the file pins down the things a length
alone would not catch -- that the route is a route at all, that the
ends are the configured ones, and the small mazes where the two
searches meet before either has room to run.
"""

import io
import unittest
from contextlib import redirect_stdout

from mazegen import Maze, MazeConfig, MazeGenerator, MazeSolver, Path
from mazegen.solvers.astar import BidirectionalAStar, manhattan
from mazegen.solvers.bfs import BFS

# Both generators, and both settings of PERFECT: a braided maze has
# loops, so it is the one where two shortest routes of equal length
# can exist and the two sides can meet away from either of them.
GENERATORS = ("backtracker", "kruskal")
# Kept small on purpose: the sweep below is 72 mazes and generating
# one costs more than solving it several times over -- 4s at 40x25
# against 0.2s at 21x13. ``test_a_long_maze`` carries the one big
# one, which is where the two frontiers have room to run past each
# other and the bound in _step is what stops them.
SIZES = ((8, 8), (15, 11), (21, 13))
SEEDS = range(6)


def build(width: int, height: int, generator: str,
          perfect: bool, seed: int) -> tuple[MazeConfig, Maze]:
    """A maze of that shape, with the config that made it."""
    config = MazeConfig(
        width, height, (0, 0), (width - 1, height - 1),
        perfect=perfect, seed=seed,
        gen_algorithm=generator, solve_algorithm="astar",
    )
    return config, MazeGenerator.from_config(config).generate()


def solve(config: MazeConfig, maze: Maze) -> Path:
    return BidirectionalAStar(config).solve(maze)


class RegisteredTest(unittest.TestCase):
    """Finding it by name, which is all the config file can do."""

    def test_it_is_in_the_registry(self) -> None:
        self.assertIs(MazeSolver.algorithms["astar"], BidirectionalAStar)

    def test_from_config_builds_one(self) -> None:
        solver = MazeSolver.from_config(
            MazeConfig(5, 5, (0, 0), (4, 4), solve_algorithm="astar"))
        self.assertIsInstance(solver, BidirectionalAStar)

    def test_the_name_is_case_and_space_insensitive(self) -> None:
        solver = MazeSolver.from_config(
            MazeConfig(5, 5, (0, 0), (4, 4), solve_algorithm="  ASTAR "))
        self.assertIsInstance(solver, BidirectionalAStar)


class HeuristicTest(unittest.TestCase):
    """Manhattan: admissible, which is what keeps the answer shortest."""

    def test_it_is_zero_only_on_the_cell_itself(self) -> None:
        self.assertEqual(manhattan((3, 4), (3, 4)), 0)
        self.assertEqual(manhattan((3, 4), (3, 5)), 1)

    def test_it_is_symmetric(self) -> None:
        self.assertEqual(manhattan((1, 2), (7, 5)), manhattan((7, 5), (1, 2)))

    def test_one_step_changes_it_by_exactly_one(self) -> None:
        goal = (9, 9)
        for step in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            here, there = (4, 4), (4 + step[0], 4 + step[1])
            self.assertEqual(
                abs(manhattan(here, goal) - manhattan(there, goal)), 1)

    def test_it_never_exceeds_the_real_route(self) -> None:
        """Admissibility, on mazes rather than in the abstract."""
        config, maze = build(21, 13, "backtracker", perfect=True, seed=1)
        route = solve(config, maze)
        for cell in route:
            self.assertLessEqual(manhattan(cell, config.exit_cell),
                                 len(route) - 1 - route.index(cell))


class RouteShapeTest(unittest.TestCase):
    """A length is only worth reading once the route is really a route."""

    def assert_walkable(self, maze: Maze, config: MazeConfig,
                        route: Path) -> None:
        self.assertEqual(route[0], config.entry_cell)
        self.assertEqual(route[-1], config.exit_cell)
        self.assertEqual(len(set(route)), len(route),
                         "a shortest route cannot use a cell twice")
        for here, there in zip(route, route[1:]):
            self.assertIn(there, [cell for cell, _side
                                  in maze.passages(*here)],
                          f"no passage from {here} to {there}")

    def test_the_route_is_walkable(self) -> None:
        for perfect in (True, False):
            for generator in GENERATORS:
                with self.subTest(generator=generator, perfect=perfect):
                    config, maze = build(21, 13, generator, perfect, seed=3)
                    self.assert_walkable(maze, config, solve(config, maze))

    def test_the_halves_are_joined_without_repeating_the_meeting(self) -> None:
        """_join starts the far half past the meeting cell."""
        config, maze = build(21, 13, "kruskal", perfect=False, seed=2)
        solver = BidirectionalAStar(config)
        route = solver.solve(maze)
        self.assertIsNotNone(solver.meeting)
        self.assertEqual(route.count(solver.meeting), 1)


class GroundTruthTest(unittest.TestCase):
    """The bar the package docstring sets: BFS's length, every time."""

    def test_it_agrees_with_bfs_everywhere(self) -> None:
        for generator in GENERATORS:
            for perfect in (True, False):
                for width, height in SIZES:
                    for seed in SEEDS:
                        with self.subTest(generator=generator,
                                          perfect=perfect,
                                          size=f"{width}x{height}",
                                          seed=seed):
                            config, maze = build(width, height, generator,
                                                 perfect, seed)
                            mine = solve(config, maze)
                            theirs = BFS(config).solve(maze)
                            self.assertEqual(len(mine), len(theirs))

    def test_a_long_maze(self) -> None:
        """Big enough that the two sides meet well away from either end."""
        for perfect in (True, False):
            with self.subTest(perfect=perfect):
                config, maze = build(31, 19, "backtracker", perfect, seed=7)
                mine = solve(config, maze)
                self.assertEqual(len(mine), len(BFS(config).solve(maze)))
                self.assertNotIn(mine[len(mine) // 2],
                                 (config.entry_cell, config.exit_cell))

    def test_mu_is_the_length_that_comes_back(self) -> None:
        """The bound proved is the route handed over, not a cousin of it."""
        for perfect in (True, False):
            with self.subTest(perfect=perfect):
                config, maze = build(21, 13, "backtracker", perfect, seed=4)
                solver = BidirectionalAStar(config)
                route = solver.solve(maze)
                self.assertEqual(solver.mu, len(route) - 1)


class SmallMazeTest(unittest.TestCase):
    """The sizes where the two sides meet before either has room to run."""

    def test_entry_is_also_the_exit(self) -> None:
        config = MazeConfig(1, 1, (0, 0), (0, 0))
        self.assertEqual(solve(config, Maze(1, 1)), [(0, 0)])

    def test_the_exit_is_the_next_cell_over(self) -> None:
        config = MazeConfig(2, 1, (0, 0), (1, 0), seed=0,
                            gen_algorithm="backtracker")
        maze = MazeGenerator.from_config(config).generate()
        self.assertEqual(solve(config, maze), [(0, 0), (1, 0)])

    def test_a_walled_maze_has_no_route(self) -> None:
        """Every wall shut, so the two sides run out without meeting."""
        config = MazeConfig(4, 4, (0, 0), (3, 3))
        with redirect_stdout(io.StringIO()) as printed:
            self.assertEqual(solve(config, Maze(4, 4)), [])
        self.assertIn("does not exist", printed.getvalue())


if __name__ == "__main__":
    unittest.main()
