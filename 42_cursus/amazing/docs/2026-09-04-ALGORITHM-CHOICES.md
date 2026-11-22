# Algorithms — the choices, and why

**Date:** 2026-09-04 — Ana (@anade-mo)

[`2026-09-01-ALGORITHMS-STUDY-PREVIEW.md`](./2026-09-01-ALGORITHMS-STUDY-PREVIEW.md) surveyed ten algorithms and deliberately picked none. This document picks them, three per side, as [`2026-08-31-MEETING-MINUTES.md`](./2026-08-31-MEETING-MINUTES.md) §4.1 resolved, and gives the reason for each — the justification that has to go in the issue request.

Every number quoted here is measured, and comes from the study: a 20x15 grid for generation, the visualizer's 22x33 grid for pathfinding.

| | Chosen |
|---|---|
| Generation | **Recursive backtracker** *(default)*, **Kruskal's**, **Recursive division** |
| Pathfinding | **BFS** *(writes the file)*, **Bidirectional A\***, **JPS** |

---

## 0. Two things the subject fixes

**Seeding.** Every algorithm here makes its choices through Python's `random`. `random.seed(42)` before generating gives the same maze every time.

**What `PERFECT` means.** `PERFECT=True` is exactly one path between any two cells: no loops, no choices, every dead-end real. `PERFECT=False` — the default — is Pac-Man mode: loops exist, routes can be chosen, dead-ends are rare or absent, and there are at least two independent routes from entry to exit. All five generators considered produce perfect mazes, so the second mode is always something done *after* generating, except in recursive division, which can do it as it goes.

---

## 1. The criterion: three paradigms, not three flavours

Criteria: 
  - Educational value and real world applications;
  - Visualization and animation potential - serves the Game story context;
  - Diversity of approach, not variants of the same idea;

MazeGen, the generator algorithms:

| Paradigm | Algorithm | The maze is built by | Data structure |
|---|---|---|---|
| Carving, one path at a time | Recursive backtracker | walking, and backing up when stuck | an explicit **stack** |
| Joining regions, in any order | Kruskal's | merging two areas that were separate | **union-find** |
| Splitting space | Recursive division | cutting a rectangle in two, with a gap | **recursion on rectangles** |

MazeSolve, the pathfinding algorithms:

| Paradigm | Algorithm | Searches by |
|---|---|---|
| Uninformed | BFS | spreading evenly in every direction |
| Informed, from both ends | Bidirectional A\* | aiming with a heuristic, from entry *and* exit |
| Informed, skipping the boring parts | JPS | jumping down corridors, stopping only at junctions |



---

## 2. Generation

### 2.1 Recursive backtracker

Description: Walk, and back up when stuck. Carves a single branch of a tree, and the backtracking is what makes it a tree rather than a line.

**Pros.** Simplest (you know stacks from C), fast, most cinematic. Single moving position at all times, so a character can follow it.

**Cons.** Biased toward long corridors. Deep recursion on large grids — use an iterative stack, since recursion depth equals cells visited and Python's limit is 1000.

Generation of a maze with the fewest dead-ends. Has a single moving position at all times, which is what an animated character would follow. Serves the game-story purpose of "a character is the maze generator". Looks good in a bush maze, carved by a gardener.

Its not a *random walk* — it is a *depth-first search*, which is the lesson of the algorithm. The path it carves is a single branch of a tree, and the backtracking is what makes it a tree rather than a line.

Cost to plan for: write it **iteratively, with an explicit stack**. The recursive form needs one frame per visited cell, and 20x15 = 300 cells already flirts with Python's default limit of 1000; a 40x40 maze in `config.txt` would crash it.

**Data structures:** stack, visited grid. **Complexity:** O(cells). Starts from a full grid and removes walls.

**For PERFECT=False:** find the dead-ends (three walls) and destroy one wall at each. DFS mazes have few dead-ends but deep ones, so every removal creates a long loop — the best starting point for a Pac-Man board.

**Used today:** roguelikes, dungeon crawlers, procedural content generation.

### 2.2 Kruskal's 

Description: Shuffle all walls, remove one by one unless both sides already connected.

**Pros.** No directional bias. Teaches Union-Find (graphs, networks, image processing).

**Cons.** The one with the most dead-ends (100 on a 20x15 grid).

- **Union-find** — the only data structure in the whole project that is not a grid, a stack or a queue. It answers "are these two cells already connected?" in near-constant time, and *that question* is the algorithm: shuffle every internal wall, remove the wall whenever the two sides belong to different regions, skip it when they are already joined. The maze is finished when everything is one region.

- **No directional bias at all** — measured, the only one of the three with none. Kruskal's leans nowhere, which is the cleanest demonstration that *bias is a property of the algorithm and not of the randomness*. 

- **Its animation is visually appealing**: walls pop open all over the grid, gives the impression of an inevitable nature force-like event. It has no moving position to follow. 34% straight corridors, pleasent even texture.

Cost to plan for: **100 dead-ends**. In `PERFECT=False` that is the heaviest braid of the three — nearly four times the backtracker's. It costs no extra *code*, since the braid is one shared pass, but it is the case that decides how the braid must be written: it has to walk every dead-end.

**Data structures:** shuffled wall list, union-find. **Complexity:** O(walls) effectively. Starts from a full grid and removes walls.

**For PERFECT=False:** there is a second option here that the others do not have — when a wall is skipped because both sides are already joined, remove it anyway with some probability. That makes loops during generation, with no second pass. Kruskal's even spread means the dead-ends are evenly spread too, so braiding gives well-distributed loops.

**Used today:** network design, circuit layout, clustering.

### 2.3 Recursive division

Description: open room, slice it with a wall that has one gap, recurse on the halves.

**Pros.** Fastest, most elegant animation, easy, natural for rooms.

**Cons.** Most predictable, trivially easy to solve. 

- it **starts from an open grid and adds walls**, so it exercises `build_wall`, which nothing else in the project would ever call;
- **divide and conquer**: cut the rectangle with a wall, punch one gap, recurse into both halves. It is the only one of the three whose structure is the *recursion itself*.
- it is the cheapest route to `PERFECT=False` **with no braiding pass at all**: punch two gaps in a wall instead of one and the loops are there from the start;
- 32% straight corridors and a roomy, grid-like texture: it reads as architecture, serves the game narratives that needs to start with a all open maze.

Cost to plan for: it needs an open grid first (every internal wall destroyed) before walls go up, and it is the one generator whose recursion depth is *not* the number of cells — it halves each time, so ~log₂ of the size, and it can stay recursive without risking the stack.

**Data structures:** recursion only. **Complexity:** O(cells). Starts from an empty grid and adds walls.

**For PERFECT=False:** punch two gaps per wall instead of one. Each extra gap is an immediate loop, so this is the only generator that needs no second pass at all — the gap count is controlled during generation.

**Used today:** game room layouts (Zelda). Divide-and-conquer generally: quicksort, KD-trees, BSP.

---

## 3. Alternative option, for your consideration:

**Wilson's** — the only provably unbiased one (uniform spanning tree), which is the strongest education material of the five. Left out on cost: **2,425 random-walk steps to produce 725 carves** on 726 cells, the slowest by a wide margin, and the hardest to get right (loop-erased walks). 

---

## 4. Pathfinding

### 4.1 BFS — the one that writes the output file

The subject wants the shortest path in the output file, as a sequence of N/E/S/W letters. Every step in this maze costs 1, and on an unweighted graph BFS returns a shortest path.

**This is the risk control for the whole list**: the two bonuses are the clever ones, and a pathfinder that is *quietly* wrong writes a wrong path into the file, where the analyzer will find it at the defense. BFS's answer is the ground truth, so the test for both others is one line — *same length as BFS, on every maze, for every seed*.

**Data structures:** deque, visited set, parent dict. **Complexity:** O(V + E). **Animation:** a uniform circle spreading from the entry.

**Used today:** social networks, web crawlers, GPS, broadcasting.

### 4.2 Bidirectional A\* — heuristic and two frontiers at once

Two ideas in one algorithm: aim with a heuristic, and search from both ends until the two searches meet.

- The **Manhattan heuristic is admissible** on a 4-connected grid — it never overestimates, since every step changes the Manhattan distance by exactly 1 — which is precisely why the answer stays shortest.
- Two frontiers halve the *depth* each search reaches, and since the area explored grows with the square of the depth, meeting in the middle explores far less than one search going all the way. It is the **fewest cells scanned in practice** of everything measured.
- It is the best of the three to watch: two coloured frontiers growing towards each other and touching.

Cost to plan for — the one real trap: **stopping the moment the two frontiers touch does not give a shortest path.** The first meeting can be through a cell that neither side has settled cheaply yet. Worth knowing before treating that as settled: the visualizer checks mid-level, which the textbook says is unsafe, and across **600 test mazes — perfect and braided — it never returned a longer path than BFS**. So the trap is real in theory and has not been reproduced here. 

**Data structures:** two heapqs, two g dicts, two parent dicts, two closed sets. **Complexity:** O(V log V), fewest cells of anything measured. **Animation:** two narrow ellipses aiming at each other.

**Used today:** Google Maps, long-distance game AI, robotics.

### 4.3 JPS — the one that skips corridors

Instead of expanding cell by cell, run in a straight line until something happens — a junction, a wall, the goal — and queue only that cell. A maze is mostly corridor and a little junction, so most of what A\* expands is a cell with nothing to decide.

Measured against A\*: **241 expansions to 406 on a perfect maze, 325 to 470 braided**. Two things behind those numbers are worth knowing before quoting them:

- **It is expansions, not work.** The same runs scanned 807 cells against 852 — 5% fewer. The saving is in queue operations; the jump still reads every cell it passes. Measure wall-clock once both exist.
- **The saving is bigger on a perfect maze than in Pac-Man mode** (41% against 31%), because open space gives more turning options and jumps end sooner — the opposite of how JPS is usually sold.

**Our grid needs its own jump rule, and this is measured.** Ours stops at every cell where a turn is possible.

**Data structures:** A\*'s, plus the jump logic. Same worst case. Uniform-cost grids only — every step costs 1 here. **Used today:** grid games with large open maps.

---


## 5. Open questions for the next meeting

- How many dead-ends does `PERFECT=False` leave? *Proposal: zero* — it satisfies the subject, matches Pac-Man, and is the one value the analyzer can check exactly. It also makes Kruskal's 100 dead-ends a non-issue rather than a tuning problem.

- Where does the "42" go, given the pattern is 7x5 and the smallest maze that can hold it is 9x7? It is carved **after** braiding?  So the closed cells it writes are not braided open again?
