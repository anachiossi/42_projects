#!/usr/bin/env python3
"""Entry point called with python3 a_maze_ing.py config.txt"""

import random
import sys
from dataclasses import replace

from app.config import MazeConfigError, read_config
from app.render.terminal import (CARVE_DELAY, CARVE_EVERY, PATH_DELAY,
                                 SEEN_DELAY, SEEN_EVERY, WALL_COLOURS,
                                 Drawing, TooSmallTerminal, animate,
                                 apply_wall_colour, colour_menu, read_key,
                                 redraw, seen_frames, show)
from mazegen import MazeConfig, MazeGenerator, MazeSolver, Paths

TITLE = "A-MAZE-ING"

ANIMATION = True


def searched(solver: MazeSolver, animating: bool) -> Paths:
    """The cells the search went through, when they are wanted.

    Only the animation draws them. With it off the route is shown on
    its own, which is what P meant before there was a search to show.
    """
    return solver.get_visited() if animating else ()


def message_for(drawing: Drawing, config: MazeConfig,
                algorithm: str, solver: str,
                animating: bool) -> list[str]:
    state = "on" if animating else "off"
    return [
        f"generation: {algorithm}",
        f"solver: {solver}",
        f"resolution: {config.width}x{config.height}",
        f"{drawing.cell_width} wide",
        f"R  redraw   P  path   C  colour   Q  quit   A  animate ({state})",
    ]


def in_terminal(config: MazeConfig, generator: MazeGenerator,
                solver: MazeSolver, output_file: str) -> int:
    """Draw the maze in the terminal, and run until the user quits."""
    if generator.config.seed is None:
        generator.config.seed = random.randrange(1 << 30)
    maze = generator.generate()
    path = solver.solve(maze)
    maze.save(output_file, path)

    animating = ANIMATION
    drawing = Drawing(title=TITLE)
    drawing.message = message_for(
        drawing, config, generator.name, solver.name, animating)

    try:
        if not sys.stdin.isatty():
            show(maze, drawing)
            return 0
        if animating:
            animate(generator.carve(), replace(drawing, paths=()),
                    delay=CARVE_DELAY, every=CARVE_EVERY)
            maze = generator.maze

        while True:
            drawing.message = message_for(
                drawing, config, generator.name, solver.name, animating)
            redraw(maze, drawing)

            key = read_key()
            if key in ("a", "A"):
                animating = not animating
                # A route already on screen keeps matching what the
                # action bar now says, rather than waiting for P.
                if drawing.paths:
                    drawing.visited = searched(solver, animating)
            if key in ("q", "Q", "\x03", "\x1b"):
                break
            if key in ("r", "R"):
                generator.config.seed = random.randrange(1 << 30)
                # Cleared before the carve, not after it: the frames
                # below are drawn from this same drawing, and a search
                # left in it marks cells of a grid being replaced.
                drawing.visited = ()
                if animating:
                    animate(generator.carve(), replace(drawing, paths=()),
                            delay=CARVE_DELAY, every=CARVE_EVERY)
                    maze = generator.maze
                else:
                    maze = generator.generate()
                # A new maze is a new way through it, so the route is
                # asked for again whether or not it is on screen: turning
                # it on later would otherwise draw the old maze's path.
                path = solver.solve(maze)
                maze.save(output_file, path)
                if drawing.paths:
                    drawing.paths = (path,)
                    # The old maze's search would otherwise stay up,
                    # marking cells of a grid that is gone.
                    drawing.visited = searched(solver, animating)
            if key in ("p", "P"):
                if drawing.paths:
                    drawing.paths = ()
                    drawing.visited = ()
                else:
                    # Set before the route, so the animation below
                    # draws the search under it from the first frame.
                    drawing.visited = searched(solver, animating)
                    drawing.paths = (path,)
                    if animating:
                        # The search spreads first and stays up; then
                        # the route is drawn over the cells it came
                        # from, which is the order they happened in.
                        animate(seen_frames(maze, replace(drawing,
                                                          paths=())),
                                delay=SEEN_DELAY, every=SEEN_EVERY,
                                reveal=False)
                        animate((maze,), drawing, delay=PATH_DELAY)
            if key in ("c", "C"):
                print(colour_menu(maze, drawing))
                choice = read_key()
                if choice in "12345":
                    apply_wall_colour(drawing, WALL_COLOURS[int(choice) - 1])

    except TooSmallTerminal as error:
        # The window can be made smaller while the program is running,
        # so this is asked on every frame rather than once at the start.
        print(f"error: {error}", file=sys.stderr)
        return 1

    return 0


def main() -> int:
    """Read the config, then draw the maze in the terminal."""
    arguments = sys.argv[1:]
    if len(arguments) != 1:
        print("Usage: python3 a_maze_ing.py config.txt", file=sys.stderr)
        return 1

    try:
        config, output_file = read_config(arguments[0])
        generator = MazeGenerator.from_config(config)
        solver = MazeSolver.from_config(config)
        return in_terminal(config, generator, solver, output_file)
    except (MazeConfigError, ValueError, OSError, TooSmallTerminal) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
