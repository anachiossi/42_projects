# Minutes of Meeting — A-Maze-ing

**2026-08-31** — Leo (@ltamiano) and Ana (@anade-mo). Notes by Ana.

---

## 1. Purpose

Code Review & Milestones Planning

## 2. Agenda

1. GitHub practices — feedback and best practices
2. Clarification of code concepts
3. Next steps and division of work
4. Parallel workstreams
    - MazeGen and MazeSolve
         - GenAlgos and Pathfinders
         - Algorithm choice criteria
         - PacMan futureproofing
    - Render
         - Mlx package vs. terminal rendering — scope boundary
         - Rendering skins and gameplay storylines
         - Animation impact of algoritmhms  
5. Project's deadline goal: 2026-09-09

---

## 3. Next steps and division of work

Ana's responsible for rendering and the MLX package, while Leo is responsible for the implementation of the maze generator. 
The two workstreams are codependent, therefore they should be parallel and efficient.
To assist Leo in algorithm implementation, Ana will provide a simple renderer in terminal, so that Leo can test the algorithms without waiting for the MLX package to be ready.
Ana can choose the algorithms to implement, but she will need to justify the choice in the issue request. 

## 4. Discussion and resolutions

### 4.1 Algorithm portfolio — scope of implementation

Ana will be presenting a porfolio of algorithms, the study is documented in 
(`2026-09-01-ALGORITHMS-STUDY-PREVIEW.md`)
and an web app was developed with AI for the preview in 
(`algorithm-visualizer.html`).
The subject mandates only one working generator, multiple for bonus points. 

Points raised:
- All algorithms will be visible by animation. 
- They should be distinc for each other and serve education purposes.
- Algorithm's choice need to be justified in the issue request of implementation.
- Reminder: for the bonus we need to set the `ALGORITHM` config key.
- MazeGen may consider PacMan minimal requirements (if feasible) for futureproofing.

**Resolution:** - Quantity agreed, not final decision on final algorithm list.
    3 Algorithms for MazeGen
    3 Algorithms for MazeSolve (pathfinder)


### 4.2 Rendering interface — scope of implementation

Both rendering workstreams (terminal and MLX) will be implemented, prioritizing the terminal renderer for testing purposes. The MLX package will serve as final deliver, multiple skins are considered. 

Points raised:
- Terminal renderer could already mimic Pac Man, for style futureproofing.
- Priority for a minimal terminal renderer.
- Brainstorming of skins for MLX included gamestory:
    - Garden maze —> extruded bushes on grass, 3D feel of tall hedges, cast shadows, 2 characters inspired in the film Edward Scissorhands. He carves the maze and a girl can escape it. Needs mazegen algorithms that start with full walls and carve the maze.
    - Ocean maze -> bg color is tourquoise, walls are rocks under the sea. Character can be a diver, a swiming guy or a boat. They leave white trails in the water. Looks nicer if the mazegen starts will zero walls only water, and rocks apear as the walls. Can be less 3D and more 2D.
- Both storylines have ambitious textures, colors and characters.
- Skin changes can be triggered by a hidden seed number. As an easter egg.
- Skin style could be triggered by the algorithm choice.

**Resolution:** 
    First, complete the minimal pac man style terminal renderer. Then access the feasibility of the two storylines and decide which one is easier to implement first. Garden has the challenge of 3D feel and shadows, Ocean has the challenge of water texture and light. Check how to import .png textures in MLX. If both are feasible, implement both, otherwise choose the one that is easier to implement first.

### 4.3 Clarification of code concepts

Intflag type in Python. Direction class as both navigation and wall encoding.

Points raised:
- Coordinate convention — `(col, row)`
- intflag type is helpful for hexadecimal conversion and bitwise operations.

**Resolution:** Direction class will be used for both navigation and wall sensing, no need to divide into two classes.

### 4.4 Parallel workstreams

The two streams are codependent: the generator needs something to draw
with, and the renderer needs something to draw. The terminal renderer is
the first deliverable, so Leo can test the gen algorithms.

Leo owns `src/mazegen/`, Ana owns `app/render/` and the algorithm
portfolio. They meet at two methods only: `Maze.get_dim()` and
`Maze.get_wall()`.

**Resolution:** Ana delivers a minimal terminal renderer first so Leo is
not blocked on MLX. Neither stream changes `Maze`'s public signatures
without agreeing it in the issue first, since both sides compile against
them.


## 5. Next meeting

To fix the final algorithm list. 
Project deadline: **2026-09-09**.

---

## Appendix — references

- `docs/2026-08-28-INTRO.md` — initial design note
- `docs/2026-08-29-SCAFFOLD.md` — project structure and conventions
- `docs/2026-09-01-ALGORITHMS-STUDY-PREVIEW.md` — algorithm survey, with measurements
- `docs/algorithm-visualizer.html` — interactive comparison tool

