"""Minimalist rendering on terminal.

The maze is read, never written: asked for its size, its walls, its
ends and its frozen cells. Line by line, top to bottom, left to right,
appended to a string.

A maze of WIDTH x HEIGHT becomes ``2 * HEIGHT + 1`` lines::

    ┏━━━┳━━━━━━━┓      a wall line: junctions, and the walls above
    ┃ E │       ┃      a cell line: walls beside the cells, and their
    ┣━━━┿━━━┓   ┃                   insides
    ┃       │  X┃
    ┗━━━┻━━━┻━━━┛      one last wall line closes the bottom

The border is drawn first, since the outside of a maze is always
walled. Every wall left over stands between two cells, so each cell
draws only its N and W sides: drawing all four would draw every wall
twice, and the sides no cell draws are exactly the border::

    ┌───┬───┐     A, B, C and D each draw their own N and W.
    │ A │ B │
    ├───┼───┤     B's E and D's S belong to no cell. They are the
    │ C │ D │     border, drawn first.
    └───┴───┘

The maze sits in a canvas with a head for the title and a tail for
messages. Colour is not optional: an ``Element`` is a symbol and its
ink together, and ink only — one character holds one thing.

A cell walled in on all four sides is drawn like any other; the "42"
shows as the boxes it is made of.
"""

import re
import shutil
import sys
import termios
import textwrap
import time
import tty
from dataclasses import dataclass, field, replace
from typing import Iterable, Sequence

import rainbow
from mazegen import Cell, Direction, Maze, Path, Paths

# What a cell on a path knows: the sides it links to, and the element
# of the path that drew it.
Link = tuple[int, "JoinedElement"]

# A cell is this many characters wide unless asked otherwise. Three,
# because a terminal character is about twice as tall as it is wide:
# at 1 a square maze comes out flattened.
CELL_WIDTH = 3


@dataclass(frozen=True)
class Element:
    """A symbol, and the ink it is written in.

    ``ink`` is an ANSI code as it appears inside the escape sequence
    that wraps the text: ``"38;5;N"`` writes the symbol in colour N
    of the 256-colour palette. ``rainbow`` turns a name into one of
    these, so nothing here is a bare number.

    Empty ink means the terminal's own colour.
    """

    symbol: str = " "
    ink: str = ""


@dataclass(frozen=True)
class JoinedElement(Element):
    """The symbol depends on the neighbours.

    Walls, borders and the path are all lines, so which piece to use
    — ``─``, ``┐``, ``┼`` — is known only once the neighbours are.
    ``joints`` holds the sixteen pieces, indexed by the sides they
    reach towards with the bits ``Direction`` defines: N = 1, E = 2,
    S = 4, W = 8. So index 10 is E | W, a horizontal line; index 0
    touches nothing and is blank.
    """

    joints: str = " " * 16

    def __post_init__(self) -> None:
        """Refuse a table that cannot answer every junction."""
        if len(self.joints) != 16:
            raise ValueError(
                f"joints needs one symbol per junction, that is 16, "
                f"got {len(self.joints)}"
            )

    def piece(self, sides: int) -> str:
        """What piece to use for a junction with those sides."""
        return self.joints[sides]

    def straight(self, direction: Direction) -> str:
        """What piece to use for a straight line in that direction?"""
        if direction in (Direction.N, Direction.S):
            return self.joints[Direction.N | Direction.S]
        return self.joints[Direction.E | Direction.W]


@dataclass(frozen=True)
class Frame:
    """The canvas around the maze: head, tail and sides."""

    head: int = 2
    tail: int = 3
    sides: int = 2


@dataclass(frozen=True)
class TerminalSkin:
    """A reusable set of elements.

    A new look is a new instance of this class, with the same elements
    but different symbols and colours.
    """

    floor: Element
    wall: JoinedElement
    border: JoinedElement
    entry: Element
    exit: Element
    paths: tuple[JoinedElement, ...]
    title: Element
    message: Element
    meeting: JoinedElement | None = None
    char1: Element | None = None
    char2: Element | None = None
    forty_two: Element | None = None
    seen: tuple[Element, ...] = ()


STANDARD_SKIN = TerminalSkin(
    floor=Element(" "),
    wall=JoinedElement(ink=rainbow.ink("chambray"),
                       joints=" ╵╶└╷│┌├╴┘─┴┐┤┬┼"),
    border=JoinedElement(ink=rainbow.ink("cloud"),
                         joints=" ╹╺┗╻┃┏┠╸┛━┷┓┨┯╋"),
    paths=(JoinedElement(ink=rainbow.ink("gamboge"),
                         joints="•╵╶╰╷│╭├╴╯─┴╮┤┬┼"),
           JoinedElement(ink=rainbow.ink("manganese"),
                         joints="•╵╶╰╷│╭├╴╯─┴╮┤┬┼")),
    meeting=JoinedElement(ink=rainbow.ink("verdigris"),
                          joints="•╵╶╰╷│╭├╴╯─┴╮┤┬┼"),
    entry=Element("⬤", ink=rainbow.ink("peppermint")),
    exit=Element("⚑", ink=rainbow.ink("madder")),
    title=Element(ink=rainbow.ink("snow")),
    message=Element(ink=rainbow.ink("stone")),
    char1=Element("இ", ink=rainbow.ink("cadmium")),
    char2=Element("&", ink=rainbow.ink("flamingo")),
    forty_two=Element("ဩ", ink=rainbow.ink("robin")),
    seen=(Element("✱", ink=rainbow.ink("glacier")),
          Element("✱", ink=rainbow.ink("lilac"))),
)

FRAME = Frame()


@dataclass
class Drawing:
    """Everything a rendering needs to draw.

    ``paths`` is a list of routes, drawn one element each; a single
    search is a list of one. ``visited`` is the same shape and holds
    what a solver went through on its way to them, one sequence per
    search front — a route is drawn over its own search, never under
    it. The two ends are not here — they are the maze's, and ``render``
    reads them from it. ``skin`` left alone means the standard one.
    """

    paths: Paths = ()
    visited: Paths = ()
    char1: Cell | None = None
    char2: Cell | None = None
    title: str = ""
    message: Sequence[str] = ()
    skin: TerminalSkin | None = None
    frame: Frame = FRAME
    cell_width: int = CELL_WIDTH

    def drawn_with(self) -> TerminalSkin:
        """The skin named, or the one every drawing gets by default."""
        return self.skin if self.skin is not None else STANDARD_SKIN


# ``Direction`` is an IntFlag, and every ``&`` or ``|`` on one builds a
# fresh enum member — tens of thousands per frame, which was more than
# half the time spent drawing. The bit work below is done on these
# plain ints instead; ``Direction`` stays the language at the edges.
NORTH = int(Direction.N)
EAST = int(Direction.E)
SOUTH = int(Direction.S)
WEST = int(Direction.W)


def maze_wall_above(maze: Maze, col: int, row: int) -> bool:
    """Is there a horizontal wall segment above cell (col, row)?

    The N and S edges of the maze are walled by definition; inside,
    the segment is the north wall of the cell below it.
    """
    width, height = maze.get_dim()
    if not (0 <= col < width and 0 <= row <= height):
        return False
    if row in (0, height):
        return True
    return maze.get_wall(col, row, Direction.N)


def maze_wall_left(maze: Maze, col: int, row: int) -> bool:
    """Is there an upright wall segment left of cell (col, row)?

    The W and E edges of the maze are walled by definition; inside,
    the segment is the west wall of the cell to its right.
    """
    width, height = maze.get_dim()
    if not (0 <= row < height and 0 <= col <= width):
        return False
    if col in (0, width):
        return True
    return maze.get_wall(col, row, Direction.W)


def path_sides(path: Path) -> dict[Cell, int]:
    """The shape of the path line in each cell.

    The path is the route the solver walks, entry to exit. For every
    cell of it, the sides it links to added together — the same
    sixteen-piece index the walls are drawn with.
    """
    sides: dict[Cell, int] = {}
    for index, cell in enumerate(path):
        reaches = 0
        for other in (path[index - 1] if index else None,
                      path[index + 1] if index + 1 < len(path) else None):
            if other is None:
                continue
            for side in Direction:
                step_col, step_row = side.step()
                if (cell[0] + step_col, cell[1] + step_row) == other:
                    reaches |= side
        sides[cell] = reaches
    return sides


def merge_paths(paths: Paths, elements: Sequence[JoinedElement],
                meeting: JoinedElement | None = None,
                goal: Cell | None = None) -> dict[Cell, Link]:
    """Merge several paths into one link per cell.

    Each path takes its own element, round again if there are more
    paths than elements. Where paths share a cell the sides add up,
    so it links every way any of them leaves by.

    Once solved, every cell of every path takes the meeting element
    instead — two colours right up to that step, one from it on.
    Solved means a cell more than one path runs through, or a path
    that has reached ``goal``.
    """
    sides_of: dict[Cell, int] = {}
    walked_by: dict[Cell, set[int]] = {}
    for index, path in enumerate(paths):
        for cell, sides in path_sides(path).items():
            sides_of[cell] = sides_of.get(cell, 0) | sides
            walked_by.setdefault(cell, set()).add(index)

    solved = (any(len(walkers) > 1 for walkers in walked_by.values())
              or (goal is not None and goal in sides_of))
    links: dict[Cell, Link] = {}
    for cell, sides in sides_of.items():
        if solved and meeting is not None:
            element = meeting
        else:
            element = elements[min(walked_by[cell]) % len(elements)]
        links[cell] = (sides, element)
    return links


def paint(text: str, element: Element) -> str:
    """Return the text wrapped in colors."""
    if not text:
        return text
    return f"\033[{element.ink}m{text}\033[0m" if element.ink else text


# ---------------------------------------------------------------------
# The maze itself: one wall line and one cell line per row.


def maze_junction(maze: Maze, col: int, row: int,
                  skin: TerminalSkin) -> tuple[str, Element]:
    """Return the piece that belongs where four walls could meet."""

    width, height = maze.get_dim()
    sides = 0
    if maze_wall_left(maze, col, row - 1):
        sides |= NORTH
    if maze_wall_left(maze, col, row):
        sides |= SOUTH
    if maze_wall_above(maze, col, row):
        sides |= EAST
    if maze_wall_above(maze, col - 1, row):
        sides |= WEST
    on_frame = col in (0, width) or row in (0, height)
    element = skin.border if on_frame else skin.wall
    return element.piece(sides), element


def _wall_line(maze: Maze, row: int, plan: Drawing,
               links: dict[Cell, Link]) -> str:
    """Return the horizontal line that runs above a row of cells.

    Every junction, and between them the wall the cells above and
    below share. There is one more of these lines than there are
    rows: row 0 is the top of the maze, row ``height`` the bottom.

    Where there is no wall the floor shows through, and the path
    crosses the opening north to south.
    """

    skin, cell_width = plan.drawn_with(), plan.cell_width
    width, height = maze.get_dim()
    out = []
    for col in range(width + 1):
        symbol, element = maze_junction(maze, col, row, skin)
        out.append(paint(symbol, element))
        if col == width:
            break
        if maze_wall_above(maze, col, row):
            element = skin.border if row in (0, height) else skin.wall
            out.append(paint(element.straight(Direction.E) * cell_width,
                             element))
        else:
            out.append(_wall_opening(
                skin, cell_width, Direction.N,
                _path_through(links, (col, row), Direction.N)))
    return "".join(out)


def _cell_line(maze: Maze, row: int, plan: Drawing,
               links: dict[Cell, Link],
               marks: dict[Cell, Element],
               seen: dict[Cell, Element]) -> str:
    """Return the line through a row: its walls and its centres.

    The wall standing beside each cell, then the inside of the cell —
    floor, the path, whatever mark stands there. One per row.

    An opening here is in an upright wall, so the path crosses it
    east to west.
    """
    skin = plan.drawn_with()
    width, _ = maze.get_dim()
    out = []
    for col in range(width + 1):
        if maze_wall_left(maze, col, row):
            element = skin.border if col in (0, width) else skin.wall
            out.append(paint(element.straight(Direction.N), element))
        else:
            out.append(_wall_opening(
                skin, 1, Direction.W,
                _path_through(links, (col, row), Direction.W)))
        if col == width:
            break
        out.append(_cell_body(col, row, plan, links, marks, seen))
    return "".join(out)


def maze_line(maze: Maze, char_row: int, plan: Drawing,
              links: dict[Cell, Link],
              marks: dict[Cell, Element],
              seen: dict[Cell, Element]) -> str:
    """Return one line of the maze, whichever kind it is.

    A 2x2 maze is five lines, and they alternate::

        char_row 0   ┏━━━┯━━━┓   even — the walls above, and junctions
        char_row 1   ┃ E │   ┃   odd  — the walls beside, and centres
        char_row 2   ┠───┼───┨   even
        char_row 3   ┃   │ X ┃   odd
        char_row 4   ┗━━━┷━━━┛   even

    The centres are where anything that is not a wall goes: the
    floor, the path, E and X. ``divmod`` gives both answers at once —
    which kind of line, and which row of cells it belongs to.
    """
    row, kind = divmod(char_row, 2)
    if kind:
        return _cell_line(maze, row, plan, links, marks, seen)
    return _wall_line(maze, row, plan, links)


def _cell_body(col: int, row: int, plan: Drawing,
               links: dict[Cell, Link],
               marks: dict[Cell, Element],
               seen: dict[Cell, Element]) -> str:
    """Return the inside of one cell: everything that is not a wall.

    The middle, where a mark or the path stands, and a stretch either
    side of it where the path runs out towards the cells it links
    to::

        "   "    nothing here
        " E "    entry symbol stands here
        "───"    the path runs straight through
        "─╮ "    the path arrives from the west and turns south
    """
    skin, cell_width = plan.drawn_with(), plan.cell_width
    cell = (col, row)
    middle = cell_width // 2
    sides, element = links.get(cell, (0, None))
    return (_path_run(middle, skin, element if sides & Direction.W else None)
            + _cell_centre(skin, cell, marks, links, seen)
            + _path_run(cell_width - middle - 1, skin,
                        element if sides & Direction.E else None))


def _cell_centre(skin: TerminalSkin, cell: Cell,
                 marks: dict[Cell, Element],
                 links: dict[Cell, Link],
                 seen: dict[Cell, Element]) -> str:
    """Return the character in the middle of a cell.

    A mark beats the path, the path beats a cell the search went
    through, and that beats the floor. The order is the point: every
    cell of a route was searched first, so a search drawn over the
    route would rub it out.
    """
    if cell in marks:
        return paint(marks[cell].symbol, marks[cell])
    if cell in links:
        sides, element = links[cell]
        return paint(element.piece(sides), element)
    if cell in seen:
        return paint(seen[cell].symbol, seen[cell])
    return paint(skin.floor.symbol, skin.floor)


def _path_run(span: int, skin: TerminalSkin,
              path: JoinedElement | None) -> str:
    """Return the path running out of a cell, or the floor."""
    if path is None:
        return paint(skin.floor.symbol * span, skin.floor)
    return paint(path.straight(Direction.E) * span, path)


def _wall_opening(skin: TerminalSkin, span: int, through: Direction,
                  crossing: JoinedElement | None) -> str:
    """What to draw where a wall is missing: the path, or the floor.

    ``through`` is the way the opening is passed — north for a gap in
    a wall that runs across, west for one that stands upright. The
    caller says which, since the span cannot tell them apart.
    """
    if crossing is None:
        return paint(skin.floor.symbol * span, skin.floor)
    piece = crossing.straight(through)
    middle = span // 2
    return (paint(skin.floor.symbol * middle, skin.floor)
            + paint(piece, crossing)
            + paint(skin.floor.symbol * (span - middle - 1), skin.floor))


def _path_through(links: dict[Cell, Link], cell: Cell,
                  side: Direction) -> JoinedElement | None:
    """Return the path that crosses the opening on that side, if any.

    Asked from both sides: this cell may link that way, or the cell
    beyond may link back. Either answer is the same crossing, and the
    element that comes with it says which path made it.
    """
    step_col, step_row = side.step()
    other = (cell[0] + step_col, cell[1] + step_row)
    sides, element = links.get(cell, (0, None))
    if sides & side:
        return element
    sides, element = links.get(other, (0, None))
    if sides & side.opposite():
        return element
    return None


def wrap(text: str, width: int) -> list[str]:
    """Break a message into as many lines as it takes to keep all of it.

    A message that already fits comes back untouched, spacing and
    all: the action bar spaces its keys out on purpose. Only when it
    does not fit is that spacing spent, a word at a time.
    """
    if width < 1 or len(text) <= width:
        return [text]
    return textwrap.wrap(text, width, break_long_words=True,
                         break_on_hyphens=False) or [""]


def message_lines(message: Sequence[str], canvas: int,
                  frame: Frame) -> list[str]:
    """The lines under the maze: every message, wrapped to the canvas.

    ``frame.tail`` is a floor, not a count. Fewer lines are padded
    out with blanks so nothing jumps between frames; more make the
    tail taller rather than being cut.
    """
    lines = [line for text in message for line in wrap(text, canvas)]
    return lines + [""] * (frame.tail - len(lines))


def band(text: str, width: int, element: Element) -> str:
    """Return one line of canvas, with the text centred on it."""
    text = text[:width]
    left = (width - len(text)) // 2
    return (" " * left + paint(text, element)
            + " " * (width - left - len(text)))


# ---------------------------------------------------------------------
# And finally but not least, the render function.


def drawing_size(width: int, height: int, cell_width: int = CELL_WIDTH,
                 frame: Frame = FRAME) -> tuple[int, int]:
    """Return the columns and lines a drawing of that maze would take.

    Every cell needs its own width plus one character for the wall
    beside it, and one more closes the right-hand side; every row
    needs a line for its cells and one for the wall above, and one
    more closes the bottom. The frame is added to both.
    """
    return (width * (cell_width + 1) + 1 + 2 * frame.sides,
            2 * height + 1 + frame.head + frame.tail)


def mark_cells(maze: Maze, plan: Drawing,
               skin: TerminalSkin) -> dict[Cell, Element]:
    """What stands on a cell instead of the floor, in drawing order.

    The "42" first, so that an end or a character standing on one of
    its cells is drawn over the filling rather than under it; then the
    two characters; then the entry and the exit, which always win.
    """
    marks: dict[Cell, Element] = {}
    if skin.forty_two is not None:
        for filled in maze.get_frozen():
            marks[filled] = skin.forty_two
    for cell, element in ((plan.char1, skin.char1), (plan.char2, skin.char2),
                          (maze.get_entry(), skin.entry),
                          (maze.get_exit(), skin.exit)):
        if cell is not None and element is not None:
            marks[cell] = element
    return marks


def seen_cells(plan: Drawing, skin: TerminalSkin) -> dict[Cell, Element]:
    """What a cell the search has been through looks like.

    One element per front, in the order the fronts are given, so the
    two halves of a search from both ends are told apart the same way
    two routes are. A cell claimed by more than one front keeps the
    first that reached it.
    """
    seen: dict[Cell, Element] = {}
    if not skin.seen:
        return seen
    for index, front in enumerate(plan.visited):
        element = skin.seen[index % len(skin.seen)]
        for cell in front:
            seen.setdefault(cell, element)
    return seen


def canvas(body: Sequence[str], plan: Drawing, skin: TerminalSkin,
           columns: int) -> str:
    """Set the drawn maze in its canvas: title above, messages below.

    The head is blank lines with the title centred in the middle of
    them, the maze is padded either side by ``frame.sides``, and the
    tail is the messages, each centred on the same width.
    """
    frame = plan.frame
    side = " " * frame.sides
    out = [band("", columns, skin.title) for _ in range(frame.head)]
    if plan.title and frame.head:
        out[frame.head // 2] = band(plan.title, columns, skin.title)
    out.extend(side + drawn + side for drawn in body)
    for text in message_lines(plan.message, columns, frame):
        out.append(band(text, columns, skin.message))
    return "\n".join(out)


class TooSmallTerminal(Exception):
    """The window cannot hold the drawing of that maze."""


def too_small(needed: tuple[int, int],
              space: tuple[int, int]) -> TooSmallTerminal:
    """The refusal, said in a way the reader can act on.

    Which way it is short, by how much, and what to do about it. A
    terminal is made bigger by making its text smaller, which is not
    the first thing that comes to mind when a program says there is
    not enough room — so it is the thing this says out loud.
    """
    short = []
    if needed[0] > space[0]:
        short.append(f"{needed[0] - space[0]} columns")
    if needed[1] > space[1]:
        short.append(f"{needed[1] - space[1]} lines")
    return TooSmallTerminal(
        f"this maze needs {needed[0]} columns x {needed[1]} lines, and "
        f"the window is {space[0]} x {space[1]} — {' and '.join(short)} "
        f"short. Press ctrl and minus to make the text smaller: the "
        f"window then holds more of both."
    )


def render(maze: Maze, drawing: Drawing | None = None) -> str:
    """Draw a maze and return it as one string, ready to print.

    Four steps: what the path looks like in every cell, what stands on
    a cell, the maze drawn line by line consulting both, and the
    canvas set around it.

    Raises ``TooSmallTerminal`` when the window cannot hold it. The
    drawing is never shrunk to make it fit — a maze is what was asked
    for, and half of one is not an answer.
    """
    plan = drawing or Drawing()
    skin = plan.drawn_with()
    width, height = maze.get_dim()
    columns, lines = drawing_size(width, height, plan.cell_width, plan.frame)
    space = shutil.get_terminal_size()
    if columns > space.columns or lines > space.lines:
        raise too_small((columns, lines), (space.columns, space.lines))

    links = merge_paths(plan.paths, skin.paths, skin.meeting, maze.get_exit())
    marks = mark_cells(maze, plan, skin)
    seen = seen_cells(plan, skin)
    body = [maze_line(maze, char_row, plan, links, marks, seen)
            for char_row in range(2 * height + 1)]
    return canvas(body, plan, skin, columns)


def show(maze: Maze, drawing: Drawing | None = None) -> None:
    """Print a maze in colour, for a quick look."""
    print(render(maze, drawing))


def redraw(maze: Maze, drawing: Drawing | None = None) -> None:
    """Draw the maze over the last one, in a single write.

    The screen is never wiped first. The cursor goes home, each line
    erases whatever is left of the row it lands on (``\033[K``), and
    one erase at the end clears the rows a shorter drawing no longer
    covers (``\033[J``). Wiping first would leave the screen blank
    until the new frame arrived, which is what shows as a flicker.

    No trailing newline either: a frame as tall as the window would
    scroll it by a row, and the next frame would start one row high.
    """
    frame = render(maze, drawing).replace("\n", "\033[K\n")
    sys.stdout.write("\033[H" + frame + "\033[K\033[J")
    sys.stdout.flush()


def read_key() -> str:
    """Wait for one key, without requiring Enter."""
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        return sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)


WALL_COLOURS = (
    "chambray",
    "vermilion",
    "gamboge",
    "peppermint",
    "ultramarine",
)


def colour_menu(maze: Maze, drawing: Drawing) -> str:
    """Numbered swatches, centred on the same canvas as the message."""
    parts: list[str] = []
    for index, name in enumerate(WALL_COLOURS, start=1):
        swatch = rainbow.write("█", ink=name)
        parts.append(f"{index} {swatch} {name}")
    text = "  ".join(parts)
    width, height = maze.get_dim()
    columns, _ = drawing_size(width, height, drawing.cell_width,
                              drawing.frame)
    visible = len(re.sub(r"\033\[[0-9;]*m", "", text))
    left = max(0, (columns - visible) // 2)
    right = max(0, columns - left - visible)
    return " " * left + text + " " * right


def apply_wall_colour(drawing: Drawing, colour: str) -> None:
    """Repaint interior walls and the border in that colour."""
    base = drawing.drawn_with()
    ink = rainbow.ink(colour)
    drawing.skin = replace(
        base,
        wall=replace(base.wall, ink=ink),
        border=replace(base.border, ink=ink),
    )


# ---------------------------------------------------------------------
# Animation: the same drawing, again, over itself.


# How long one frame is held. Carving yields a frame for every wall it
# takes down, so it runs at the faster of the two; a route arriving is
# meant to be followed by eye.
# How long a frame is held *on top of* the time it takes to draw one.
# On a 30x30 that is already some twenty milliseconds, which paces the
# animation on its own; the pause is what a small maze needs, where a
# frame costs almost nothing.
CARVE_DELAY = 0.0
PATH_DELAY = 0.02

# One carved wall in this many is drawn. A 30x30 carve yields around a
# thousand frames and drawing one costs some twenty milliseconds, so
# every one of them is half a minute of watching a wall come down.
CARVE_EVERY = 5

# The search spreads over far fewer cells than a carve takes down
# walls — a 20x15 is around two hundred against a thousand — so every
# frame of it is drawn, and the spread is watched cell by cell.
#
# A frame of search costs about five milliseconds to draw, well under
# the twenty a carve costs, so unlike CARVE_DELAY this one cannot be
# left at nothing: the whole reveal would go by in a fifth of a
# second. The pause is what paces it, and it is shorter than
# PATH_DELAY because there are several times as many cells to get
# through.
SEEN_DELAY = 0.005
SEEN_EVERY = 1


@dataclass
class AnimationFrame:
    """One animation snapshot: maze state together with its drawing."""

    maze: Maze
    drawing: Drawing = field(default_factory=Drawing)


def path_frames(maze: Maze, drawing: Drawing) -> Iterable[AnimationFrame]:
    """Yield snapshots that reveal every path one cell at a time.

    Each route grows by a cell per frame, which is what makes two
    halves of a bidirectional search meet on screen.
    """
    length = max((len(path) for path in drawing.paths), default=0)
    for end in range(length + 1):
        prefixes = tuple(path[:end] for path in drawing.paths)
        yield AnimationFrame(maze, replace(drawing, paths=prefixes))


def seen_frames(maze: Maze, drawing: Drawing) -> Iterable[AnimationFrame]:
    """Yield snapshots that spread the search one cell at a time.

    ``path_frames`` over the other field. Every front grows together,
    a cell each per frame, so a search running from both ends is seen
    closing on itself rather than one side finishing before the other
    starts.

    The order is the solver's: whatever ``get_visited`` handed over is
    replayed as it came. A solver that keeps its cells in a set has
    nothing to replay, and its search simply arrives whole.
    """
    length = max((len(front) for front in drawing.visited), default=0)
    for end in range(length + 1):
        prefixes = tuple(front[:end] for front in drawing.visited)
        yield AnimationFrame(maze, replace(drawing, visited=prefixes))


def clear_screen() -> None:
    """Send the cursor home and wipe what is on the screen."""
    sys.stdout.write("\033[H\033[J")


def hide_cursor() -> None:
    """Take the cursor off the screen while something is animating.

    It parks at the end of the last line drawn, where it blinks and,
    if the drawing does not fit the window, appears to wander as the
    screen scrolls under it. Neither is part of the picture.
    """
    sys.stdout.write("\033[?25l")
    sys.stdout.flush()


def show_cursor() -> None:
    """Put the cursor back. Always paired with ``hide_cursor``."""
    sys.stdout.write("\033[?25h")
    sys.stdout.flush()


def _play(frame: Maze | AnimationFrame, shared: Drawing,
          delay: float, reveal: bool) -> None:
    """Draw one frame, growing its paths a cell at a time if asked."""
    if isinstance(frame, AnimationFrame):
        maze, plan = frame.maze, frame.drawing
    else:
        maze, plan = frame, shared
    snapshots = (path_frames(maze, plan)
                 if reveal and plan.paths
                 else (AnimationFrame(maze, plan),))
    for snapshot in snapshots:
        redraw(snapshot.maze, snapshot.drawing)
        time.sleep(delay)


def animate(frames: Iterable[Maze | AnimationFrame],
            drawing: Drawing | None = None,
            delay: float = 0.05, reveal: bool = True,
            every: int = 1) -> None:
    """Draw one maze after another in place, as a moving picture.

    ``frames`` may be maze states, or ``AnimationFrame`` snapshots
    carrying their own drawing; maze-only frames use the shared
    ``drawing``. A drawing with paths reveals them a cell at a time,
    unless ``reveal`` is False — which is how a finished route is
    held on screen instead of grown again from nothing.

    A generator that yields itself after each change animates too::

        animate(generator.carve(), delay=0.02)

    ``every`` draws one frame in that many and lets the rest go by. A
    carve yields after every single wall, and drawing one costs far
    more than making one, so what makes a long carve watchable is
    showing less of it rather than waiting less between frames. The
    last frame is always drawn, whatever the count says: it is the
    finished picture.

    The screen is wiped once before the first frame; every frame after
    overwrites it in place, so what is left at the end is the finished
    maze. The cursor is hidden for the duration.
    """
    if every < 1:
        raise ValueError(f"a frame in every {every} is not a number of "
                         f"frames; the least is 1")
    shared = drawing or Drawing()
    clear_screen()
    hide_cursor()
    try:
        skipped: Maze | AnimationFrame | None = None
        for number, frame in enumerate(frames):
            if number % every:
                skipped = frame
                continue
            skipped = None
            _play(frame, shared, delay, reveal)
        if skipped is not None:
            _play(skipped, shared, delay, reveal)
    finally:
        # Even on ctrl-c: a terminal left without a cursor is a
        # terminal the next program has to fix.
        show_cursor()
