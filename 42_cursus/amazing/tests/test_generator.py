"""Tests for MazeConfig and MazeGenerator."""

import time
import unittest

from mazegen import Direction, Edge, Maze, MazeConfig, MazeGenerator

# TODO: corridor constraint.


class MazeConfigTest(unittest.TestCase):
    """The settings object the generator is built from."""

    def test_it_keeps_the_values(self) -> None:
        config = MazeConfig(
            20, 15, (0, 0), (19, 14), perfect=True, seed=7,
            gen_algorithm="kruskal")
        self.assertEqual(config.width, 20)
        self.assertEqual(config.height, 15)
        self.assertEqual(config.entry_cell, (0, 0))
        self.assertEqual(config.exit_cell, (19, 14))
        self.assertTrue(config.perfect)
        self.assertEqual(config.seed, 7)
        self.assertEqual(config.gen_algorithm, "kruskal")

    def test_optional_fields_default(self) -> None:
        config = MazeConfig(4, 3, (0, 0), (3, 2))
        self.assertFalse(config.perfect)
        self.assertIsNone(config.seed)
        self.assertIsNone(config.gen_algorithm)
        self.assertIsNone(config.solve_algorithm)

    def test_a_zero_size_is_refused(self) -> None:
        with self.assertRaises(ValueError) as caught:
            MazeConfig(0, 3, (0, 0), (0, 2))
        self.assertIn("0x3", str(caught.exception))

    def test_an_exit_outside_the_maze_is_refused(self) -> None:
        with self.assertRaises(ValueError) as caught:
            MazeConfig(4, 3, (0, 0), (9, 9))
        self.assertIn("exit", str(caught.exception))


def _brute_chokes(generator: MazeGenerator) -> set[Edge]:
    """The old search: one region walk per open passage."""
    entry = generator.config.entry_cell
    exit_cell = generator.config.exit_cell
    return {edge for edge in generator.maze.open_passages()
            if exit_cell not in generator.maze.region_of(
                entry[0], entry[1], banned=edge)}


class ChokePointsTest(unittest.TestCase):
    """Bridges that cut the entry from the exit, found in linear time."""

    def test_a_corridor_is_all_chokes(self) -> None:
        config = MazeConfig(4, 1, (0, 0), (3, 0), perfect=True)
        generator = MazeGenerator.from_config(config)
        maze = Maze(4, 1, entry=(0, 0), exit=(3, 0))
        maze.destroy_wall(0, 0, Direction.E)
        maze.destroy_wall(1, 0, Direction.E)
        maze.destroy_wall(2, 0, Direction.E)
        generator.maze = maze
        chokes = set(generator._choke_points())
        self.assertEqual(chokes, {
            frozenset(((0, 0), (1, 0))),
            frozenset(((1, 0), (2, 0))),
            frozenset(((2, 0), (3, 0))),
        })

    def test_a_loop_has_no_choke(self) -> None:
        config = MazeConfig(2, 2, (0, 0), (1, 1), perfect=False)
        generator = MazeGenerator.from_config(config)
        maze = Maze(2, 2, entry=(0, 0), exit=(1, 1))
        maze.destroy_wall(0, 0, Direction.E)
        maze.destroy_wall(0, 0, Direction.S)
        maze.destroy_wall(1, 0, Direction.S)
        maze.destroy_wall(0, 1, Direction.E)
        generator.maze = maze
        self.assertEqual(generator._choke_points(), [])

    def test_it_matches_searching_every_passage(self) -> None:
        for perfect in (True, False):
            with self.subTest(perfect=perfect):
                config = MazeConfig(10, 8, (0, 0), (9, 7),
                                    perfect=perfect, seed=3)
                generator = MazeGenerator.from_config(config)
                generator.generate()
                self.assertEqual(set(generator._choke_points()),
                                 _brute_chokes(generator))

    def test_a_playable_maze_is_no_longer_slow(self) -> None:
        # The old scan was ~5s on 30x30. One DFS should stay well
        # under a second, the same order as carving the tree.
        config = MazeConfig(30, 30, (0, 0), (29, 29),
                            perfect=False, seed=1)
        generator = MazeGenerator.from_config(config)
        started = time.perf_counter()
        generator.generate()
        elapsed = time.perf_counter() - started
        self.assertLess(elapsed, 1.0)
        self.assertEqual(generator._choke_points(), [])


if __name__ == "__main__":
    unittest.main()
