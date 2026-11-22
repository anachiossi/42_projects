"""Configuration file reading."""

from mazegen import Cell, MazeConfig, forty_two

# The maze sizes the program agrees to draw. The subject sets no
# ceiling; this one is here so that a slip like WIDTH=99999 is refused
# at once rather than after a very long wait.
MIN_SIZE = 1
MAX_SIZE = 1000

TRUE_WORDS = ("true", "1", "yes")
FALSE_WORDS = ("false", "0", "no")


class MazeConfigError(Exception):
    """Error raised when the config file or keys are invalid."""


def read_config(path: str) -> tuple[MazeConfig, str]:
    """Read a ``KEY=VALUE`` file (ignores comments `#´).

    Gives back the settings the maze is built and walked with, and the
    file the maze is to be written to. OUTPUT_FILE comes back on its
    own because mazegen never writes anything: the name would be a
    field nothing in the library could use.
    """
    values = _read(path)

    width = _int(values, "width")
    height = _int(values, "height")
    entry = _point(values, "entry")
    exit_cell = _point(values, "exit")
    output_file = _need(values, "output_file")
    perfect = _bool(values, "perfect")
    # Optional: absent is not an error, it means "you choose".
    seed = _int(values, "seed") if "seed" in values else None

    _check(width, height, entry, exit_cell)

    config = MazeConfig(
        width=width,
        height=height,
        entry_cell=entry,
        exit_cell=exit_cell,
        perfect=perfect,
        seed=seed,
        gen_algorithm=values.get("gen_algorithm"),
        solve_algorithm=values.get("solve_algorithm"),
    )
    return config, output_file


def _check(width: int, height: int, entry: Cell, exit_cell: Cell) -> None:
    """Check the values describe a maze that can exist."""
    for name, size in (("WIDTH", width), ("HEIGHT", height)):
        if not MIN_SIZE <= size <= MAX_SIZE:
            raise MazeConfigError(
                f"{name} must be between {MIN_SIZE} and "
                f"{MAX_SIZE}, got {size}"
            )
    # The subject asks every maze to carry the "42". A maze with no
    # room for it is refused here rather than drawn without it: the
    # library still builds one that small, but the program will not.
    if not forty_two.fits(width, height):
        raise MazeConfigError(
            f"a {width}x{height} maze has no room for the 42: "
            f"WIDTH must be at least {forty_two.MIN_WIDTH} and "
            f"HEIGHT at least {forty_two.MIN_HEIGHT}"
        )
    for name, (col, row) in (("ENTRY", entry), ("EXIT", exit_cell)):
        if not (0 <= col < width and 0 <= row < height):
            raise MazeConfigError(
                f"{name} falls outside a {width}x{height} "
                f"maze: column {col}, row {row}"
            )
    if entry == exit_cell:
        raise MazeConfigError("ENTRY and EXIT must be different cells")


def _read(path: str) -> dict[str, str]:
    """Every KEY=VALUE line of the file, keys lowered, values as written."""
    values: dict[str, str] = {}
    try:
        with open(path, "r", encoding="utf-8") as config_file:
            for number, raw in enumerate(config_file, start=1):
                line = raw.strip()
                if not line or line.startswith("#"):
                    continue
                if "=" not in line:
                    raise MazeConfigError(
                        f"{path}, line {number}: expected KEY=VALUE"
                    )
                key, value = line.split("=", 1)
                values[key.strip().lower()] = value.strip()
    except FileNotFoundError as error:
        raise MazeConfigError(f"config file not found: {path}") from error
    except OSError as error:
        raise MazeConfigError(f"cannot read {path}: {error}") from error
    return values


def _need(values: dict[str, str], key: str) -> str:
    """The value written for that key, or say which one is missing."""
    if key not in values:
        raise MazeConfigError(f"missing mandatory key: {key.upper()}")
    return values[key]


def _int(values: dict[str, str], key: str) -> int:
    """Read a whole number, or say which key was wrong."""
    value = _need(values, key)
    try:
        return int(value)
    except ValueError as error:
        raise MazeConfigError(
            f"{key.upper()} must be a whole number, got {value!r}"
        ) from error


def _bool(values: dict[str, str], key: str) -> bool:
    """Read a boolean. An unrecognised word is an error, not False."""
    value = _need(values, key)
    if value.lower() in TRUE_WORDS:
        return True
    if value.lower() in FALSE_WORDS:
        return False
    raise MazeConfigError(
        f"{key.upper()} must be True or False, got {value!r}"
    )


def _point(values: dict[str, str], key: str) -> Cell:
    """Read a ``col,row`` pair and keep its order exactly as written.

    19,14 stays (19, 14): the 20th column, the 15th row.
    """
    value = _need(values, key)
    parts = value.split(",")
    if len(parts) != 2:
        raise MazeConfigError(
            f"{key.upper()} must be written as col,row, got {value!r}"
        )
    try:
        col, row = (int(part.strip()) for part in parts)
    except ValueError as error:
        raise MazeConfigError(
            f"{key.upper()} must be two whole numbers, got {value!r}"
        ) from error
    return col, row
