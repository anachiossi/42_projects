*This project has been created as part of the 42 curriculum by anade-mo, ltamiano.*

# A-Maze-ing

This project is all about mazes, generating mazes and computing
shortes paths within mazes. Every maze generated carries a block of
closed cells spelling **42**. The maze is drawn in the terminal.

## Instructions

To run the project it is required to have python 3.10 or newer. Run
the following makefile commands

```bash
make install
make run
```

You can also run the project directly as follows

```bash
python3 a_maze_ing.py config.txt
```

## Configuration file

TODO

## Maze Generation Algorithms

TODO

## Shortest Path Algorithms

TODO

## The mazegen module

The library can be used to generate a maze structure and compute the
shortest path and it is independent from the app graphical aspect. It
can be installed as follows.

```bash
make build
pip install mazegen-1.0.0-py3-none-any.whl
```

An example of using the library.

```python
from mazegen import MazeConfig, MazeGenerator, MazeSolver

config = MazeConfig(20, 15, (0, 0), (19, 14), seed=7,
                    gen_algorithm="kruskal", solve_algorithm="bfs")
maze = MazeGenerator.from_config(config).generate()
path = MazeSolver.from_config(config).solve(maze)

print(maze.get_dim(), maze.get_entry(), maze.get_exit(), len(path))
maze.save("maze.txt", path)
```

## Advanced features

- Rendering for both generation and solving animation
- Multiple algorithm for generation and solving

## Team and project management

TODO

## Resources

TODO

## License

MIT, see [LICENSE.md](LICENSE.md). It covers the whole repository,
including the reusable `mazegen` package.
