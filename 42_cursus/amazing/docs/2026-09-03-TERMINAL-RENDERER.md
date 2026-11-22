# Terminal renderer

**2026-09-03 — Ana (@anade-mo).** Files: `app/render/terminal.py`, `a_maze_ing.py`, `tests/test_terminal.py`, `tests/test_fit.py`.

Delivers §4.2 of the 2026-08-31 minutes: something to draw with while the generator is written, before MiniLibX.

## Contract

```python
maze.get_dim()                      # (width, height)
maze.get_wall(col, row, direction)  # True when closed
maze.get_entry()                    # cell, or None
maze.get_exit()                     # cell, or None
```

Nothing else is read; `grid` is never touched.

**`MazeView` deleted (2026-09-08).** The renderers took a `Protocol` naming the handful of methods they were allowed to call, so mypy would reject a write. Both now take the `Maze` itself: the read-only discipline was worth less than the indirection cost, and `Cell`/`Path`/`Paths` moved to `mazegen.maze`, where the generator and solver already needed them.

**Widened from two methods to four (2026-09-04).** The ends used to be handed to the renderer on the `Drawing`; they are the maze's now, since the solver walks between them and the subject's file format writes them in its footer beside the grid — a drawing carrying its own copy could disagree with the maze it was drawn from. `Drawing.entry` and `Drawing.exit` are gone rather than kept as overrides: if marking an arbitrary cell is wanted later it should get its own explicit abstraction, not a second meaning for these two. §4.4 fixes the meeting point at `get_dim` and `get_wall`, so this needs an issue against the minutes.

`tests/fake_maze.py` deleted — `Maze.randomize()` covers what it was for.

## Design

**One pass, no surface.** Lines are built top to bottom, left to right, appended to a string; every character is decided once and never revisited. A surface-based version (paint, read back, overpaint) was written first and dropped: 36 functions and 228 lines against 22 and 191, for the same output. Cost of the trade: no drawing *over* — the order things win is decided inside `_middle`, not by call order.

**Border first, then N and W.** The outside of a maze is walled by definition, so the four edges are drawn without reading the grid. What is left is interior, where every wall belongs to two cells and each cell draws only its north and west — so every wall has exactly one owner, with no last-row or last-column case. Price: a border broken in the data does not show; `Maze` refuses to open one, so it takes reaching past its API.

**Two kinds of line, one way in.**

```python
line(maze, char_row, …)     # divmod picks the kind and the row
  _horizontal_line(…)       # even: horizontal walls, junctions between
  _centre_line(…)           # odd:  upright walls, and cell centres
```

`2 * height + 1` lines; one more horizontal than there are rows, which is why `render` counts lines rather than rows.

**Junctions are computed, not read back.** `junction()` asks the four wall segments around a corner and indexes the skin's 16 pieces by the `Direction` bits (N=1, E=2, S=4, W=8). `horizontal()` / `vertical()` answer whether a segment is closed; out of range answers False, which is what leaves a corner with two arms.

## Skins

```python
Element(symbol, ink)               # one thing that can appear
JoinedElement(…, joints)           # 16 pieces, indexed by sides
TerminalSkin(floor, wall, border, entry, exit, paths, meeting,
             title, message, char1, char2)
```

Two ship: `STANDARD_SKIN` (box drawing) and `BLOCKS` (solid `█`, for cells one character wide). A new look is an instance, not a subclass — they differ in values, never behaviour.

**No backgrounds.** Every colour is ink; one character holds one thing. Colours are named through `rainbow` (`src/rainbow`, ships with the project): `ink=rainbow.ink("gamboge")`. `python3 -m rainbow` lists all 256.

**No special drawing for walled-in cells.** A cell with four walls is drawn like any other; the "42" shows as the boxes it is made of.

## Paths

`Drawing.paths` is a list of routes, `TerminalSkin.paths` a list of elements, one per route, plus `meeting`.

```python
paths_links(paths, elements, meeting, goal) -> dict[Cell, (sides, element)]
```

Sides add up where routes share a cell. **Solved** — routes sharing a cell, or any route reaching `goal` — draws *every* cell of *every* path in `meeting`. So bidirectional search is two colours until the halves touch and one colour from that frame on, and a single search turns on arrival. `Maze.get_exit()` supplies `goal`.

The sides of a cell index the same 16 pieces the walls use, so a bend in a route is chosen exactly as a bend in a wall is; the path skin uses rounded corners so the two never read alike.

## Animation

```python
animate(frames, drawing=None, delay=0.05, reveal=True)
```

A frame is a `Maze`, or an `AnimationFrame` carrying its own `Drawing`. With `reveal`, a drawing that has paths is expanded by `path_frames(maze, drawing)` — every path grows a cell per frame, which is what makes two halves meet. `reveal=False` draws the frame as it is; needed to hold a finished route, since otherwise every call regrows it.

The screen is cleared before each frame, never after, so the last frame stays up and a final `delay=0.0` costs nothing.

Generation animates the same way once the generator yields:

```python
animate(generator.carve(), plan, delay=0.02)
```

## Size

| | where | why |
|---|---|---|
| how big a drawing is — `drawing_size()` | renderer | its own arithmetic |
| which width fits — `widest_that_fits()` | renderer | the same, backwards |
| which skin suits a width — `skin_for()` | renderer | the other half of that answer |
| how big the space is | renderer, by default | the terminal is what it draws into |
| what to do when nothing fits | `a_maze_ing.py` | error, exit 1 |

Three things a caller may leave unsaid, each settled by the renderer:

```python
Drawing(fit=True)          # cell_width: the widest that fits the space
Drawing(cell_width=2)      # skin:       the one that suits that width
widest_that_fits(20, 15)   # space:      the terminal this runs in
```

**`fit` is off by default.** Fitting reads the terminal, and a drawing that did so unasked would come out differently in every window it was made in — the suite renders 59 drawings that must not depend on the window they run in. `render` calls `Drawing.fit_to(maze)` itself; it is public because whoever has to name the size *before* the drawing exists — the caption under the maze does — should get the same answer rather than working it out a second time.

**`skin=None`** means "whatever suits `cell_width`", so a line skin cannot be paired with a one-character cell by omission; naming a skin wins. Cells 3 and 2 use `STANDARD_SKIN`, 1 uses `BLOCKS`. `skin_for` sits beside `widest_that_fits` because that function can return 1, and a line skin drawn one character wide cannot turn a corner; apart, the agreement between the two was written down nowhere.

**Passing a space** still keeps the answer a pure calculation — tests hand it a size made up on the spot, and a window can hand it its own.

Lines do not scale (a cell is one line tall), so height either fits or nothing does; columns are the dial.

Earlier this arithmetic lived in `a_maze_ing.py` as well as the renderer and the two drifted twice. The decision still belongs to the glue; the arithmetic does not.

## Extending

| Want | Change |
|---|---|
| cells taller than one line | `cell_height` in both line builders |
| a visited-cells tint | needs a background on `Element`; absent by choice |
| Pac-Man pellets | `floor=Element("·")` — no code |
| a walking character | `Drawing.char1` moved per frame |
| a third route colour | one more element in `skin.paths` |

## Checking

```bash
make run          # config.txt, fitted to the terminal
make test         # 197 cases
make lint-strict  # flake8 + mypy --strict
```

Without the venv, prefix with `PYTHONPATH=src` — `mazegen` and `rainbow` live under `src/`.

## Stopgaps

`app/render/terminal_demo.py` (`make demo`) drew a `randomize()` maze and a route written out by hand for seed 168 at 10x5. **Deleted 2026-09-08**, as planned: the generator and solver have landed, so the real pipeline draws what it used to fake.
