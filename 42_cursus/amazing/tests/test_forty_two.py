"""Tests for the mandatory "42" the subject asks every maze to carry."""

import unittest

from mazegen import (Maze, MazeConfig, MazeGenerator,
                     forty_two)


class PatternTest(unittest.TestCase):
    """The shape itself, before any maze is involved."""

    def test_the_pattern_is_twenty_six_cells(self) -> None:
        self.assertEqual(len(forty_two.PATTERN), 26)

    def test_the_pattern_fills_its_stated_box(self) -> None:
        cols = {col for col, _row in forty_two.PATTERN}
        rows = {row for _col, row in forty_two.PATTERN}
        self.assertEqual(max(cols) + 1, forty_two.COLS)
        self.assertEqual(max(rows) + 1, forty_two.ROWS)
        self.assertEqual(min(cols), 0)
        self.assertEqual(min(rows), 0)

    def test_a_maze_with_no_room_is_refused(self) -> None:
        self.assertFalse(forty_two.fits(forty_two.MIN_WIDTH - 1,
                                        forty_two.MIN_HEIGHT))
        self.assertFalse(forty_two.fits(forty_two.MIN_WIDTH,
                                        forty_two.MIN_HEIGHT - 1))
        self.assertTrue(forty_two.fits(forty_two.MIN_WIDTH,
                                       forty_two.MIN_HEIGHT))

    def test_the_gap_lands_on_the_middle_column(self) -> None:
        """The 4 and the 2 sit either side of the maze's middle."""
        for width, height in ((20, 15), (40, 30), (21, 15)):
            with self.subTest(size=(width, height)):
                cols = {col for col, _row in forty_two.cells(width, height)}
                self.assertNotIn(width // 2, cols)
                self.assertIn(width // 2 - 1, cols)
                self.assertIn(width // 2 + 1, cols)

    def test_the_rows_are_shared_out_evenly(self) -> None:
        for width, height in ((20, 15), (40, 30)):
            with self.subTest(size=(width, height)):
                rows = {row for _col, row in forty_two.cells(width, height)}
                above, below = min(rows), height - 1 - max(rows)
                self.assertLessEqual(abs(above - below), 1)

    def test_the_narrowest_maze_keeps_its_border(self) -> None:
        """Only there does the border win over the middle."""
        width, height = forty_two.MIN_WIDTH, forty_two.MIN_HEIGHT
        cols = {col for col, _row in forty_two.cells(width, height)}
        self.assertEqual(min(cols), forty_two.MARGIN)
        self.assertEqual(max(cols), width - 1 - forty_two.MARGIN)

    def test_the_corridor_leaves_every_corner_free(self) -> None:
        """A Pac-Man level needs its corners, so the 42 keeps off them."""
        for width, height in ((forty_two.MIN_WIDTH, forty_two.MIN_HEIGHT),
                              (20, 15), (40, 30)):
            with self.subTest(size=(width, height)):
                chosen = forty_two.cells(width, height)
                for corner in ((0, 0), (width - 1, 0),
                               (0, height - 1), (width - 1, height - 1)):
                    self.assertNotIn(corner, chosen)


class ReserveTest(unittest.TestCase):
    """What ``reserve`` leaves behind on a blank grid."""

    def setUp(self) -> None:
        self.maze = Maze(20, 15)
        self.chosen = forty_two.reserve(self.maze)

    def test_every_cell_of_the_glyph_is_closed(self) -> None:
        for col, row in self.chosen:
            self.assertEqual(self.maze.wall_count(col, row), 4)

    def test_the_maze_remembers_which_cells_they_are(self) -> None:
        self.assertEqual(self.maze.get_frozen(), self.chosen)

    def test_a_cell_off_the_grid_is_refused(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.set_frozen([(20, 15)])

    def test_any_iterable_will_do_and_is_kept_as_a_set(self) -> None:
        """What the caller hands over cannot change the answer later."""
        handed = [(0, 0), (1, 0), (0, 0)]
        self.maze.set_frozen(handed)
        handed.append((2, 0))
        self.assertEqual(self.maze.get_frozen(), frozenset({(0, 0), (1, 0)}))


class GeneratedTest(unittest.TestCase):
    """What the generator does around the shape.

    The size is the one ``config.txt`` ships with, and it is the
    smallest place the two modes can be compared side by side.
    """

    sizes = ((20, 15), (11, 9), (30, 22))

    def _generate(self, width: int, height: int, seed: int,
                  perfect: bool) -> Maze:
        config = MazeConfig(
            width, height, (0, 0), (width - 1, height - 1),
            perfect=perfect, seed=seed)
        return MazeGenerator.from_config(config).generate()

    def test_the_glyph_survives_generation(self) -> None:
        for width, height in self.sizes:
            for perfect in (True, False):
                for seed in range(3):
                    with self.subTest(size=(width, height), seed=seed,
                                      perfect=perfect):
                        maze = self._generate(width, height, seed, perfect)
                        expected = forty_two.cells(width, height)
                        self.assertEqual(maze.get_frozen(), expected)
                        for col, row in expected:
                            self.assertEqual(maze.wall_count(col, row), 4)

    def test_everything_else_stays_reachable(self) -> None:
        """The only unreachable cells a maze may have are the glyph's.

        This is the analyzer's rule: any *other* cell nobody can get
        to is a level nobody could finish.
        """
        for width, height in self.sizes:
            for perfect in (True, False):
                for seed in range(3):
                    with self.subTest(size=(width, height), seed=seed,
                                      perfect=perfect):
                        maze = self._generate(width, height, seed, perfect)
                        glyph = maze.get_frozen()
                        playable = {(col, row)
                                    for col in range(width)
                                    for row in range(height)
                                    if (col, row) not in glyph}
                        self.assertEqual(maze.region_of(0, 0), playable)

    def test_a_maze_too_small_simply_goes_without(self) -> None:
        maze = self._generate(6, 6, 0, True)
        self.assertEqual(maze.get_frozen(), frozenset())
        self.assertEqual(len(maze.region_of(0, 0)), 36)


if __name__ == "__main__":
    unittest.main()
