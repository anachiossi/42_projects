"""Tests for Maze and Direction.

Run from the repository root::

    python3 -m unittest discover -s tests -t .
"""

import random
import unittest
from itertools import product

from mazegen import Direction, Maze
from mazegen.maze import ALL_WALLS

SIDES = (Direction.N, Direction.E, Direction.S, Direction.W)


def cells_of(pairs: list[tuple[tuple[int, int], Direction]]
             ) -> list[tuple[int, int]]:
    """Just the cells from a (cell, side) list, for the cases about cells."""
    return [cell for cell, _side in pairs]


class MazeTest(unittest.TestCase):
    """Common helpers for the cases below."""

    def open_maze(self, width: int, height: int) -> Maze:
        """Return a maze whose every wall is down.

        Building starts from a closed grid, so a test that wants to
        watch a wall appear needs the opposite starting point.
        """
        maze = Maze(width, height)
        for col, row in product(range(width), range(height)):
            maze.grid[col][row] = Direction(0)
        return maze

    def snapshot(self, maze: Maze) -> list[list[int]]:
        """Copy the grid as plain numbers, to compare it later."""
        return [[int(cell) for cell in column] for column in maze.grid]

    def cells(self, maze: Maze) -> list[tuple[int, int]]:
        """List every cell of the maze."""
        width, height = maze.get_dim()
        return list(product(range(width), range(height)))

    def assert_coherent(self, maze: Maze) -> None:
        """Assert both sides of every wall agree, as the subject asks."""
        for col, row in self.cells(maze):
            for side in SIDES:
                step_col, step_row = side.step()
                other_col, other_row = col + step_col, row + step_row
                if (other_col, other_row) not in maze:
                    continue
                self.assertEqual(
                    maze.get_wall(col, row, side),
                    maze.get_wall(other_col, other_row, side.opposite()),
                    f"cell ({col}, {row}) and its {side.name} neighbour "
                    f"disagree about the wall between them",
                )

    def assert_border_closed(self, maze: Maze) -> None:
        """Assert the external border is still walled all around."""
        for col, row in self.cells(maze):
            for side in SIDES:
                step_col, step_row = side.step()
                if (col + step_col, row + step_row) in maze:
                    continue
                self.assertTrue(
                    maze.get_wall(col, row, side),
                    f"the {side.name} border of ({col}, {row}) is open",
                )


class TestDirection(MazeTest):
    """The encoding the subject imposes."""

    def test_bit_of_each_side(self) -> None:
        self.assertEqual((int(Direction.N), int(Direction.E),
                          int(Direction.S), int(Direction.W)), (1, 2, 4, 8))

    def test_all_walls_is_fifteen(self) -> None:
        self.assertEqual(int(ALL_WALLS), 15)

    def test_sides_combine(self) -> None:
        self.assertEqual(int(Direction.N | Direction.W), 9)

    def test_opposite(self) -> None:
        self.assertEqual(Direction.N.opposite(), Direction.S)
        self.assertEqual(Direction.E.opposite(), Direction.W)
        self.assertEqual(Direction.S.opposite(), Direction.N)
        self.assertEqual(Direction.W.opposite(), Direction.E)

    def test_opposite_twice_returns_the_same_side(self) -> None:
        for side in SIDES:
            self.assertEqual(side.opposite().opposite(), side)

    def test_step(self) -> None:
        """row grows downwards, so going north subtracts one."""
        self.assertEqual(Direction.N.step(), (0, -1))
        self.assertEqual(Direction.E.step(), (1, 0))
        self.assertEqual(Direction.S.step(), (0, 1))
        self.assertEqual(Direction.W.step(), (-1, 0))

    def test_step_of_opposite_cancels_out(self) -> None:
        for side in SIDES:
            col, row = side.step()
            other_col, other_row = side.opposite().step()
            self.assertEqual((col + other_col, row + other_row), (0, 0))

    def test_opposite_refuses_several_sides(self) -> None:
        with self.assertRaises(KeyError):
            (Direction.N | Direction.E).opposite()

    def test_step_refuses_several_sides(self) -> None:
        with self.assertRaises(KeyError):
            (Direction.N | Direction.E).step()


class TestConstruction(MazeTest):
    """A new maze is a solid block of closed cells."""

    def test_grid_is_columns_of_cells(self) -> None:
        """grid[col][row]: 4 columns of 3 cells, not the other way."""
        maze = Maze(4, 3)
        self.assertEqual(len(maze.grid), 4)
        for column in maze.grid:
            self.assertEqual(len(column), 3)

    def test_columns_are_independent(self) -> None:
        """Writing one cell must not show up in the other columns."""
        maze = Maze(3, 3)
        maze.grid[0][0] = Direction(0)
        self.assertEqual(int(maze.grid[1][0]), 15)
        self.assertEqual(int(maze.grid[2][0]), 15)

    def test_every_cell_starts_closed(self) -> None:
        maze = Maze(4, 3)
        for col, row in self.cells(maze):
            self.assertEqual(maze.wall_count(col, row), 4)

    def test_get_dim(self) -> None:
        self.assertEqual(Maze(20, 15).get_dim(), (20, 15))

    def test_smallest_maze(self) -> None:
        maze = Maze(1, 1)
        self.assertEqual(maze.get_dim(), (1, 1))
        self.assertEqual(maze.wall_count(0, 0), 4)

    def test_a_maze_needs_a_cell(self) -> None:
        for width, height in ((0, 5), (5, 0), (-1, 5), (0, 0)):
            with self.assertRaises(ValueError):
                Maze(width, height)

    def test_a_fresh_maze_is_coherent(self) -> None:
        self.assert_coherent(Maze(5, 4))
        self.assert_border_closed(Maze(5, 4))


class TestContains(MazeTest):
    """(col, row) in maze, the public way to ask whether a cell exists."""

    def test_every_cell_of_the_grid_is_in(self) -> None:
        maze = Maze(4, 3)
        for col, row in self.cells(maze):
            self.assertTrue((col, row) in maze)

    def test_a_cell_past_the_last_one_is_out(self) -> None:
        maze = Maze(4, 3)
        self.assertFalse((4, 0) in maze)
        self.assertFalse((0, 3) in maze)
        self.assertFalse((4, 3) in maze)

    def test_negative_coordinates_are_out(self) -> None:
        """Python would read the last column instead of failing."""
        maze = Maze(4, 3)
        self.assertFalse((-1, 0) in maze)
        self.assertFalse((0, -1) in maze)

    def test_the_walls_do_not_matter(self) -> None:
        """A cell exists whether it is closed or wide open."""
        maze = self.open_maze(3, 3)
        self.assertTrue((1, 1) in maze)

    def test_the_only_cell_of_a_1x1_maze(self) -> None:
        maze = Maze(1, 1)
        self.assertTrue((0, 0) in maze)
        self.assertFalse((1, 0) in maze)


class TestEnds(MazeTest):
    """Entry and exit: the two cells the maze is walked between."""

    def test_a_maze_starts_with_no_ends(self) -> None:
        """Nothing is assumed: a bare grid is not entered anywhere."""
        maze = Maze(4, 3)
        self.assertIsNone(maze.get_entry())
        self.assertIsNone(maze.get_exit())

    def test_the_ends_can_be_given_at_construction(self) -> None:
        maze = Maze(4, 3, (0, 0), (3, 2))
        self.assertEqual(maze.get_entry(), (0, 0))
        self.assertEqual(maze.get_exit(), (3, 2))

    def test_either_end_on_its_own(self) -> None:
        """They are independent: one may be known before the other."""
        self.assertEqual(Maze(4, 3, entry=(1, 1)).get_entry(), (1, 1))
        self.assertIsNone(Maze(4, 3, entry=(1, 1)).get_exit())
        self.assertEqual(Maze(4, 3, exit=(2, 2)).get_exit(), (2, 2))
        self.assertIsNone(Maze(4, 3, exit=(2, 2)).get_entry())

    def test_the_ends_can_be_set_afterwards(self) -> None:
        """A maze carved first and given its ends after."""
        maze = Maze(4, 3)
        maze.set_entry((0, 2))
        maze.set_exit((3, 0))
        self.assertEqual(maze.get_entry(), (0, 2))
        self.assertEqual(maze.get_exit(), (3, 0))

    def test_an_end_can_be_taken_away_again(self) -> None:
        maze = Maze(4, 3, (0, 0), (3, 2))
        maze.set_entry(None)
        maze.set_exit(None)
        self.assertIsNone(maze.get_entry())
        self.assertIsNone(maze.get_exit())

    def test_an_end_outside_the_grid_is_refused(self) -> None:
        """The mistake is caught where it is made, not further down.

        A solver told to start nowhere searches nothing and a renderer
        told to mark nowhere draws nothing, and neither can say why.
        """
        for cell in ((4, 0), (0, 3), (-1, 0), (0, -1)):
            with self.subTest(cell=cell):
                with self.assertRaises(IndexError):
                    Maze(4, 3, entry=cell)
                with self.assertRaises(IndexError):
                    Maze(4, 3, exit=cell)
                with self.assertRaises(IndexError):
                    Maze(4, 3).set_entry(cell)
                with self.assertRaises(IndexError):
                    Maze(4, 3).set_exit(cell)

    def test_the_message_names_the_end_and_the_size(self) -> None:
        """Which end was wrong, and what it would have had to be."""
        with self.assertRaises(IndexError) as caught:
            Maze(4, 3, exit=(9, 9))
        self.assertIn("exit", str(caught.exception))
        self.assertIn("(9, 9)", str(caught.exception))
        self.assertIn("4x3", str(caught.exception))

    def test_a_refused_end_leaves_the_maze_alone(self) -> None:
        """The one that was already set is not lost to a bad second."""
        maze = Maze(4, 3, entry=(0, 0))
        with self.assertRaises(IndexError):
            maze.set_exit((9, 9))
        self.assertEqual(maze.get_entry(), (0, 0))
        self.assertIsNone(maze.get_exit())

    def test_the_walls_do_not_matter(self) -> None:
        """An end is a cell of the grid, open or closed."""
        maze = self.open_maze(3, 3)
        maze.set_entry((1, 1))
        self.assertEqual(maze.get_entry(), (1, 1))

    def test_both_ends_may_be_the_same_cell(self) -> None:
        """Maze does not police that: the config file already does."""
        maze = Maze(4, 3, (2, 2), (2, 2))
        self.assertEqual(maze.get_entry(), maze.get_exit())


class TestBounds(MazeTest):
    """Every method refuses a cell outside the grid."""

    def setUp(self) -> None:
        self.maze = Maze(3, 2)

    def test_get_wall(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.get_wall(3, 0, Direction.N)

    def test_build_wall(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.build_wall(9, 9, Direction.N)

    def test_destroy_wall(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.destroy_wall(9, 9, Direction.N)

    def test_across(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.across(3, 0, Direction.E)

    def test_neighbours(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.neighbours(3, 0)

    def test_closed_walls(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.closed_walls(3, 0)

    def test_passages(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.passages(0, 2)

    def test_region_of(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.region_of(3, 0)

    def test_wall_count(self) -> None:
        with self.assertRaises(IndexError):
            self.maze.wall_count(3, 0)

    def test_negative_coordinates_are_refused(self) -> None:
        """Python would read the last column instead of failing."""
        for col, row in ((-1, 0), (0, -1)):
            with self.assertRaises(IndexError):
                self.maze.get_wall(col, row, Direction.N)

    def test_the_message_names_the_cell_and_the_size(self) -> None:
        with self.assertRaises(IndexError) as caught:
            self.maze.get_wall(3, 0, Direction.N)
        self.assertIn("(3, 0)", str(caught.exception))
        self.assertIn("3x2", str(caught.exception))


class TestGetWall(MazeTest):
    """Reading a single side of a cell."""

    def test_a_fresh_cell_is_closed_on_every_side(self) -> None:
        maze = Maze(3, 3)
        for side in SIDES:
            self.assertTrue(maze.get_wall(1, 1, side))

    def test_reads_the_side_it_is_asked_for(self) -> None:
        maze = self.open_maze(3, 3)
        maze.grid[1][1] = Direction.N | Direction.W
        self.assertTrue(maze.get_wall(1, 1, Direction.N))
        self.assertTrue(maze.get_wall(1, 1, Direction.W))
        self.assertFalse(maze.get_wall(1, 1, Direction.E))
        self.assertFalse(maze.get_wall(1, 1, Direction.S))

    def test_returns_a_bool_not_a_direction(self) -> None:
        self.assertIsInstance(Maze(3, 3).get_wall(1, 1, Direction.N), bool)


class TestBuildWall(MazeTest):
    """Building keeps both sides of a wall in step."""

    def test_sets_the_side_asked_for(self) -> None:
        maze = self.open_maze(3, 3)
        maze.build_wall(1, 1, Direction.N)
        self.assertEqual(int(maze.grid[1][1]), int(Direction.N))

    def test_sets_the_opposite_side_of_the_neighbour(self) -> None:
        maze = self.open_maze(3, 3)
        maze.build_wall(1, 1, Direction.N)
        self.assertEqual(int(maze.grid[1][0]), int(Direction.S))

    def test_leaves_the_other_cells_alone(self) -> None:
        maze = self.open_maze(3, 3)
        maze.build_wall(1, 1, Direction.N)
        self.assertEqual(int(maze.grid[0][0]), 0)
        self.assertEqual(int(maze.grid[2][2]), 0)

    def test_on_the_border_touches_only_the_cell(self) -> None:
        """There is no cell north of row 0, and it must not crash."""
        maze = self.open_maze(3, 3)
        maze.build_wall(0, 0, Direction.N)
        self.assertTrue(maze.get_wall(0, 0, Direction.N))

    def test_several_sides_at_once(self) -> None:
        maze = self.open_maze(3, 3)
        maze.build_wall(1, 1, ALL_WALLS)
        self.assertEqual(maze.wall_count(1, 1), 4)
        self.assertTrue(maze.get_wall(1, 0, Direction.S))
        self.assertTrue(maze.get_wall(2, 1, Direction.W))
        self.assertTrue(maze.get_wall(1, 2, Direction.N))
        self.assertTrue(maze.get_wall(0, 1, Direction.E))

    def test_building_twice_changes_nothing(self) -> None:
        maze = self.open_maze(3, 3)
        maze.build_wall(1, 1, Direction.N)
        before = self.snapshot(maze)
        maze.build_wall(1, 1, Direction.N)
        self.assertEqual(before, self.snapshot(maze))

    def test_no_side_at_all_is_a_no_operation(self) -> None:
        maze = self.open_maze(3, 3)
        before = self.snapshot(maze)
        maze.build_wall(1, 1, Direction(0))
        self.assertEqual(before, self.snapshot(maze))

    def test_the_maze_stays_coherent(self) -> None:
        maze = self.open_maze(4, 4)
        maze.build_wall(1, 1, Direction.N | Direction.E)
        maze.build_wall(2, 2, ALL_WALLS)
        self.assert_coherent(maze)


class TestDestroyWall(MazeTest):
    """Destroying is the mirror of building, minus the border."""

    def test_clears_the_side_asked_for(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.N)
        self.assertFalse(maze.get_wall(1, 1, Direction.N))

    def test_clears_the_opposite_side_of_the_neighbour(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.N)
        self.assertFalse(maze.get_wall(1, 0, Direction.S))

    def test_leaves_the_other_sides_alone(self) -> None:
        """Only one bit goes down, the cell does not become 0."""
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.N)
        self.assertTrue(maze.get_wall(1, 1, Direction.E))
        self.assertTrue(maze.get_wall(1, 1, Direction.S))
        self.assertTrue(maze.get_wall(1, 1, Direction.W))

    def test_several_sides_at_once(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, ALL_WALLS)
        self.assertEqual(int(maze.grid[1][1]), 0)

    def test_destroying_twice_changes_nothing(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.N)
        before = self.snapshot(maze)
        maze.destroy_wall(1, 1, Direction.N)
        self.assertEqual(before, self.snapshot(maze))

    def test_no_side_at_all_is_a_no_operation(self) -> None:
        maze = Maze(3, 3)
        before = self.snapshot(maze)
        maze.destroy_wall(1, 1, Direction(0))
        self.assertEqual(before, self.snapshot(maze))

    def test_a_border_wall_is_refused(self) -> None:
        """The subject requires walls all around the maze."""
        maze = Maze(3, 3)
        for (col, row), side in ((((0, 0)), Direction.N),
                                 (((0, 0)), Direction.W),
                                 (((2, 2)), Direction.S),
                                 (((2, 2)), Direction.E)):
            with self.assertRaises(ValueError):
                maze.destroy_wall(col, row, side)

    def test_the_message_names_the_side_and_the_cell(self) -> None:
        with self.assertRaises(ValueError) as caught:
            Maze(3, 3).destroy_wall(0, 0, Direction.N)
        self.assertIn("N", str(caught.exception))
        self.assertIn("(0, 0)", str(caught.exception))

    def test_a_refused_request_changes_nothing(self) -> None:
        """S and E are legal here, but N is not: nothing may move."""
        maze = Maze(3, 3)
        before = self.snapshot(maze)
        with self.assertRaises(ValueError):
            maze.destroy_wall(0, 0, ALL_WALLS)
        self.assertEqual(before, self.snapshot(maze))

    def test_build_after_destroy_returns_to_closed(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.W)
        maze.build_wall(1, 1, Direction.W)
        self.assertEqual(maze.wall_count(1, 1), 4)
        self.assertEqual(maze.wall_count(0, 1), 4)

    def test_the_maze_stays_coherent(self) -> None:
        maze = Maze(4, 4)
        maze.destroy_wall(1, 1, Direction.N | Direction.E)
        maze.destroy_wall(2, 2, ALL_WALLS)
        self.assert_coherent(maze)


class TestNeighbours(MazeTest):
    """Which cells exist around another one, walls ignored."""

    def test_an_inner_cell_has_four(self) -> None:
        maze = Maze(3, 3)
        self.assertEqual(sorted(cells_of(maze.neighbours(1, 1))),
                         [(0, 1), (1, 0), (1, 2), (2, 1)])

    def test_a_border_cell_has_three(self) -> None:
        self.assertEqual(len(Maze(3, 3).neighbours(0, 1)), 3)

    def test_a_corner_has_two(self) -> None:
        maze = Maze(3, 3)
        self.assertEqual(sorted(cells_of(maze.neighbours(0, 0))),
                         [(0, 1), (1, 0)])

    def test_the_only_cell_of_a_1x1_maze_has_none(self) -> None:
        self.assertEqual(Maze(1, 1).neighbours(0, 0), [])

    def test_the_walls_are_ignored(self) -> None:
        """A closed maze and an open one answer the same thing."""
        closed = Maze(3, 3)
        self.assertEqual(closed.neighbours(1, 1),
                         self.open_maze(3, 3).neighbours(1, 1))

    def test_every_cell_returned_exists(self) -> None:
        maze = Maze(4, 3)
        for col, row in self.cells(maze):
            for cell in cells_of(maze.neighbours(col, row)):
                self.assertTrue(cell in maze)

    def test_the_side_leads_to_the_cell_it_comes_with(self) -> None:
        """The whole reason the side is returned: it must be the right one."""
        maze = Maze(4, 3)
        for col, row in self.cells(maze):
            for (other_col, other_row), side in maze.neighbours(col, row):
                step_col, step_row = side.step()
                with self.subTest(cell=(col, row), side=side.name):
                    self.assertEqual((col + step_col, row + step_row),
                                     (other_col, other_row))


class TestPassages(MazeTest):
    """Which cells can be reached, walls taken into account."""

    def test_a_closed_maze_reaches_nothing(self) -> None:
        maze = Maze(3, 3)
        for col, row in self.cells(maze):
            self.assertEqual(maze.passages(col, row), [])

    def test_an_open_passage_is_returned(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.E)
        self.assertEqual(maze.passages(1, 1), [((2, 1), Direction.E)])

    def test_several_open_passages(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.E)
        maze.destroy_wall(1, 1, Direction.S)
        self.assertEqual(sorted(cells_of(maze.passages(1, 1))),
                         [(1, 2), (2, 1)])

    def test_the_passage_works_both_ways(self) -> None:
        maze = Maze(3, 3)
        maze.destroy_wall(1, 1, Direction.E)
        self.assertEqual(maze.passages(2, 1), [((1, 1), Direction.W)])

    def test_passages_are_neighbours(self) -> None:
        maze = Maze(4, 3)
        maze.destroy_wall(1, 1, ALL_WALLS)
        for col, row in self.cells(maze):
            self.assertLessEqual(set(maze.passages(col, row)),
                                 set(maze.neighbours(col, row)))

    def test_an_open_border_still_returns_no_outside_cell(self) -> None:
        """destroy_wall refuses it, but the grid is checked anyway."""
        maze = Maze(3, 3)
        maze.grid[0][0] &= ~Direction.N
        self.assertEqual(maze.passages(0, 0), [])

    def test_the_only_cell_of_a_1x1_maze_reaches_nothing(self) -> None:
        self.assertEqual(Maze(1, 1).passages(0, 0), [])


class TestInternalWalls(MazeTest):
    """Every wall between two cells, offered once."""

    def test_each_wall_comes_up_once(self) -> None:
        """Both cells share it, so counting from both would double it."""
        maze = Maze(4, 3)
        seen = [frozenset(((col, row), (col + side.step()[0],
                                        row + side.step()[1])))
                for (col, row), side in maze.internal_walls()]
        self.assertEqual(len(seen), len(set(seen)))

    def test_how_many_there_are(self) -> None:
        """A w x h grid has h(w-1) upright walls and w(h-1) flat ones."""
        for width, height in ((4, 3), (1, 1), (5, 1), (2, 7)):
            with self.subTest(size=(width, height)):
                self.assertEqual(len(list(Maze(width, height)
                                          .internal_walls())),
                                 height * (width - 1) + width * (height - 1))

    def test_the_border_is_never_offered(self) -> None:
        """It has no cell behind it and never opens."""
        maze = Maze(4, 3)
        for (col, row), side in maze.internal_walls():
            step_col, step_row = side.step()
            with self.subTest(cell=(col, row), side=side.name):
                self.assertTrue((col + step_col, row + step_row) in maze)

    def test_a_1x1_maze_has_none(self) -> None:
        self.assertEqual(list(Maze(1, 1).internal_walls()), [])


class TestRegionOf(MazeTest):
    """Every cell reachable from one, walls taken into account."""

    def test_a_closed_maze_reaches_only_itself(self) -> None:
        maze = Maze(3, 3)
        for col, row in self.cells(maze):
            self.assertEqual(maze.region_of(col, row), {(col, row)})

    def test_an_open_maze_reaches_everything(self) -> None:
        maze = self.open_maze(3, 3)
        self.assertEqual(maze.region_of(0, 0), set(self.cells(maze)))

    def test_it_stops_at_the_walls(self) -> None:
        """Two cells joined to each other and to nothing else."""
        maze = Maze(3, 3)
        maze.destroy_wall(0, 0, Direction.E)
        self.assertEqual(maze.region_of(0, 0), {(0, 0), (1, 0)})
        self.assertEqual(maze.region_of(1, 0), {(0, 0), (1, 0)})

    def test_everyone_in_a_region_sees_the_same_region(self) -> None:
        """Being joined is mutual, so the answer cannot depend on where
        it was asked from."""
        maze = self.open_maze(4, 3)
        maze.build_wall(1, 0, ALL_WALLS)
        for col, row in self.cells(maze):
            with self.subTest(cell=(col, row)):
                for other in maze.region_of(col, row):
                    self.assertEqual(maze.region_of(*other),
                                     maze.region_of(col, row))

    def test_a_1x1_maze_is_its_own_region(self) -> None:
        self.assertEqual(Maze(1, 1).region_of(0, 0), {(0, 0)})

    def test_a_banned_passage_is_treated_as_closed(self) -> None:
        maze = Maze(3, 1)
        maze.destroy_wall(0, 0, Direction.E)
        maze.destroy_wall(1, 0, Direction.E)
        banned = frozenset({(0, 0), (1, 0)})
        self.assertEqual(maze.region_of(0, 0, banned=banned), {(0, 0)})
        self.assertEqual(maze.region_of(1, 0, banned=banned), {(1, 0), (2, 0)})


class TestAcross(MazeTest):
    """The cell a side points at."""

    def test_it_follows_the_side(self) -> None:
        maze = Maze(3, 3)
        self.assertEqual(maze.across(1, 1, Direction.E), (2, 1))
        self.assertEqual(maze.across(1, 1, Direction.N), (1, 0))

    def test_it_may_leave_the_grid(self) -> None:
        maze = Maze(3, 3)
        self.assertEqual(maze.across(0, 0, Direction.W), (-1, 0))
        self.assertFalse(maze.across(0, 0, Direction.W) in maze)


class TestClosedWalls(MazeTest):
    """Internal sides still walled; the border is not listed."""

    def test_a_closed_centre_has_four(self) -> None:
        self.assertEqual(len(Maze(3, 3).closed_walls(1, 1)), 4)

    def test_a_corner_omits_the_border(self) -> None:
        self.assertEqual(set(Maze(3, 3).closed_walls(0, 0)),
                         {Direction.E, Direction.S})

    def test_an_open_centre_has_none(self) -> None:
        self.assertEqual(self.open_maze(3, 3).closed_walls(1, 1), [])


class TestDeadEnds(MazeTest):
    """Cells with exactly three walls."""

    def test_a_closed_maze_has_none(self) -> None:
        self.assertEqual(Maze(3, 3).dead_ends(), [])

    def test_the_ends_of_a_corridor(self) -> None:
        maze = Maze(3, 1)
        maze.destroy_wall(0, 0, Direction.E)
        maze.destroy_wall(1, 0, Direction.E)
        self.assertEqual(maze.dead_ends(), [(0, 0), (2, 0)])


class TestOpenPassages(MazeTest):
    """Open interior passages, each once."""

    def test_a_closed_maze_has_none(self) -> None:
        self.assertEqual(Maze(3, 3).open_passages(), set())

    def test_each_opening_is_one_passage(self) -> None:
        maze = Maze(2, 1)
        maze.destroy_wall(0, 0, Direction.E)
        self.assertEqual(maze.open_passages(), {frozenset({(0, 0), (1, 0)})})


class TestWallCount(MazeTest):
    """How many of a cell's four walls stand."""

    def test_a_fresh_cell_has_four(self) -> None:
        maze = Maze(3, 3)
        for col, row in self.cells(maze):
            self.assertEqual(maze.wall_count(col, row), 4)

    def test_an_open_cell_keeps_only_its_border_walls(self) -> None:
        maze = self.open_maze(3, 3)
        self.assertEqual(maze.wall_count(1, 1), 0)

    def test_it_follows_the_walls_destroyed(self) -> None:
        maze = Maze(3, 3)
        for gone, side in enumerate((Direction.E, Direction.S), start=1):
            maze.destroy_wall(1, 1, side)
            with self.subTest(side=side.name):
                self.assertEqual(maze.wall_count(1, 1), 4 - gone)

    def test_four_means_every_wall_stands(self) -> None:
        """The count and the four walls themselves must agree."""
        maze = Maze(4, 4)
        maze.destroy_wall(2, 2, Direction.N)
        for col, row in self.cells(maze):
            with self.subTest(cell=(col, row)):
                self.assertEqual(
                    maze.wall_count(col, row) == 4,
                    all(maze.get_wall(col, row, side) for side in SIDES),
                )


class TestRandomize(MazeTest):
    """Every internal wall is thrown up or down on its own."""

    def randomized(self, width: int, height: int,
                   probability: float = 0.5) -> Maze:
        """Return a randomized maze, with a fixed seed."""
        random.seed(42)
        maze = Maze(width, height)
        maze.randomize(probability)
        return maze

    def test_the_walls_stay_coherent(self) -> None:
        self.assert_coherent(self.randomized(6, 5))

    def test_the_border_stays_closed(self) -> None:
        self.assert_border_closed(self.randomized(6, 5))

    def test_some_walls_are_up_and_some_are_down(self) -> None:
        """Guard against a run that would close or open everything."""
        maze = self.randomized(6, 5)
        walls = [maze.get_wall(col, row, side)
                 for col, row in self.cells(maze) for side in SIDES]
        self.assertIn(True, walls)
        self.assertIn(False, walls)

    def test_a_probability_of_one_closes_every_cell(self) -> None:
        maze = self.randomized(4, 3, 1.0)
        for col, row in self.cells(maze):
            self.assertEqual(maze.wall_count(col, row), 4)

    def test_a_probability_of_zero_opens_every_internal_wall(self) -> None:
        maze = self.randomized(4, 3, 0.0)
        for col, row in self.cells(maze):
            self.assertEqual(sorted(maze.passages(col, row)),
                             sorted(maze.neighbours(col, row)))

    def test_the_starting_maze_does_not_matter(self) -> None:
        """A carved maze and a fresh one are randomized the same way."""
        carved = Maze(5, 4)
        carved.destroy_wall(1, 1, ALL_WALLS)
        carved.destroy_wall(2, 2, Direction.N)
        random.seed(7)
        carved.randomize()
        random.seed(7)
        fresh = Maze(5, 4)
        fresh.randomize()
        self.assertEqual(carved.grid, fresh.grid)

    def test_the_same_seed_gives_the_same_maze(self) -> None:
        self.assertEqual(self.randomized(5, 4).grid,
                         self.randomized(5, 4).grid)

    def test_a_1x1_maze_has_no_internal_wall(self) -> None:
        maze = Maze(1, 1)
        maze.randomize()
        self.assertEqual(maze.wall_count(0, 0), 4)

    def test_a_probability_outside_zero_and_one_is_refused(self) -> None:
        for probability in (-0.1, 1.1, 2.0):
            with self.assertRaises(ValueError):
                Maze(3, 3).randomize(probability)


class TestRandomCarving(MazeTest):
    """The invariants hold after a long run of random operations."""

    def carved(self) -> Maze:
        """Carve a 12x9 maze at random, with a fixed seed."""
        random.seed(42)
        maze = Maze(12, 9)
        for _ in range(400):
            col, row = random.randrange(12), random.randrange(9)
            try:
                maze.destroy_wall(col, row, random.choice(SIDES))
            except ValueError:
                pass          # a border wall, refused on purpose
        for _ in range(60):
            maze.build_wall(random.randrange(12), random.randrange(9),
                            random.choice(SIDES))
        return maze

    def test_walls_agree_between_neighbours(self) -> None:
        self.assert_coherent(self.carved())

    def test_the_border_stays_closed(self) -> None:
        self.assert_border_closed(self.carved())

    def test_reachability_is_symmetric(self) -> None:
        """If b can be reached from a, then a can be reached from b."""
        maze = self.carved()
        for col, row in self.cells(maze):
            for cell in cells_of(maze.passages(col, row)):
                self.assertIn((col, row), cells_of(maze.passages(*cell)))

    def test_no_cell_outside_the_grid_is_ever_returned(self) -> None:
        maze = self.carved()
        for col, row in self.cells(maze):
            for cell in cells_of(maze.passages(col, row)):
                self.assertTrue(cell in maze)

    def test_something_was_actually_carved(self) -> None:
        """Guard against a test that would pass on an untouched maze."""
        maze = self.carved()
        self.assertTrue(any(maze.wall_count(col, row) != 4
                            for col, row in self.cells(maze)))


if __name__ == "__main__":
    unittest.main()
