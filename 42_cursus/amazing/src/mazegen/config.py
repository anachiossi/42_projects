"""Settings for generating and solving a maze.

One object serves both. The generator and the solver are asked about
the same maze, between the same two cells; the only thing that differs
is which algorithm each of them runs, so each reads its own name and
ignores the other's.

This is not the program's ``config.txt`` reader; that lives in ``app/``
and translates into this.
"""

from .maze import Cell


class MazeConfig:
    """Size, ends, and how to build and walk a maze.

    Both algorithm names are optional: ``None`` means whichever the
    generator or the solver runs by default.
    """

    def __init__(self, width: int, height: int,
                 entry_cell: Cell, exit_cell: Cell,
                 perfect: bool = False, seed: int | None = None,
                 gen_algorithm: str | None = None,
                 solve_algorithm: str | None = None) -> None:
        if width < 1 or height < 1:
            raise ValueError(
                f"a maze needs at least one cell, got {width}x{height}"
            )
        for label, cell in (("entry", entry_cell), ("exit", exit_cell)):
            col, row = cell
            if not (0 <= col < width and 0 <= row < height):
                raise ValueError(
                    f"{label} {cell} falls outside a {width}x{height} maze"
                )
        self.width = width
        self.height = height
        self.entry_cell = entry_cell
        self.exit_cell = exit_cell
        self.perfect = perfect
        self.seed = seed
        self.gen_algorithm = gen_algorithm
        self.solve_algorithm = solve_algorithm
