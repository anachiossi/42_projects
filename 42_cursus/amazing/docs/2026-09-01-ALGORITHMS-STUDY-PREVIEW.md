# Algorithms — Study Preview

**Date:** 2026-09-01

- Interactive visualizer: [`algorithm-visualizer.html`](./algorithm-visualizer.html)

This document surveys **every algorithm under consideration**. It does not
pick any of them. The choices, and the reasoning behind them, go in a
separate document.

Numbers below are measured, not quoted: generation on a 20x15 grid,
pathfinding on the visualizer's 22x33 grid from `(0,0)` to the opposite
corner, averaged over 150-300 runs.

---

## Part 1 — Generation

**All five produce perfect mazes.** Measured: zero loops in every one.


| Algorithm | Loops | Dead ends | Straight corridors | Bias | Animation |
|---|---|---|---|---|---|
| Recursive backtracker (DFS) | 0 | 27 | 42% | Long corridors | Snake carving |
| Kruskal's | 0 | 100 | 34% | None | Popcorn merges |
| Prim's | 0 | 94 | 50% | Radial | Crystal growth |
| Wilson's | 0 | 85 | 41% | None (uniform) | Chaos, then snap |
| Recursive division | 0 | 84 | 32% | Grid-like | Room splitting |

### Consequence for PERFECT=False

`PERFECT=False` is the **default** mode and needs the opposite of what all
five give: at least two independent routes, and few or no dead-ends.
Kruskal's alone leaves 100 dead-ends on a 20x15 grid.

So a second phase is required — *braiding*: walk the dead-ends and remove
one more wall at each. Every wall removed creates exactly one loop and
eliminates one dead-end. Remove a few to satisfy "at least two independent
routes"; remove them all for the zero-dead-end bonus, which
`maze_analyzer.py --max-dead-ends 0` verifies.

    generate perfect  ->  braid if not PERFECT  ->  carve the 42  ->  write

`PERFECT=True` simply skips the braid. One generator, two modes.

### Notes per algorithm

**Recursive backtracker** — by far the fewest dead-ends (27), so the least
braiding needed. Has a single moving position at all times, which is what
an animated character would follow. Write it iteratively with an explicit
stack: recursion depth equals cells visited, and Python's limit is 1000.

**Kruskal's** — no directional bias, teaches union-find. Its animation has
no walker to follow: walls vanish all over the grid at once.

**Prim's** — most straight corridors (50%), spiky short dead-ends, radial
growth from the seed.

**Wilson's** — the only one that is provably unbiased (uniform spanning
tree), which is strong README material. Measured cost on 726 cells: 2,425
random-walk steps producing 725 carves. Slowest, hardest to implement.

**Recursive division** — the odd one out: starts open and *adds* walls, so
it cannot drive a carving animation. Easiest to make non-perfect, since
punching two gaps per wall instead of one creates loops during generation
with no second pass.

---

## Part 2 — Pathfinding

Unweighted grid, every step costs 1. All five return shortest paths.

| Algorithm | Structure | Shortest? | Notes |
|---|---|---|---|
| BFS | queue | yes | the baseline; explores uniformly |
| Bidirectional BFS | 2 queues | yes | two frontiers meet in the middle |
| A* | priority queue | yes | Manhattan heuristic, admissible |
| Bidirectional A* | 2 priority queues | yes | fewest cells in practice |
| JPS | priority queue + jumps | yes | skips corridors, queues junctions |

Measured, A* against JPS:

| Maze | A* expansions | JPS expansions | A* cells scanned | JPS cells scanned |
|---|---|---|---|---|
| Perfect | 406 | 241 (-41%) | 852 | 807 |
| Braided (Pac-Man) | 470 | 325 (-31%) | 1078 | 1074 |

---


## Still to decide

Recorded here so the choices document has a checklist:

- which generator, and whether more than one is supported (`ALGORITHM` is
  already an optional config key)
- braiding strategy for `PERFECT=False`, and how many dead-ends to leave
- which solver writes the shortest path into the output file
- where the "42" pattern is placed: minimum pattern 7x5, minimum maze 9x7
