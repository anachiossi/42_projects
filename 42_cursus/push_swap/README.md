*This project has been created as part of the 42 curriculum by anade-mo and gsantoni.*

---

# push_swap

## Description

push_swap is a sorting algorithm project from the 42 curriculum. The goal is to sort a stack of integers using two stacks (`a` and `b`) and a restricted set of operations, producing the **shortest possible sequence of moves**.

The project enforces a deep understanding of algorithmic complexity by requiring four distinct sorting strategies, each targeting a different complexity class, selected automatically based on the disorder of the input.

Available operations:

| Operation | Description |
|-----------|-------------|
| `sa` / `sb` | Swap the top two elements of stack a / b |
| `ss` | `sa` and `sb` at the same time |
| `pa` / `pb` | Push top of b onto a / top of a onto b |
| `ra` / `rb` | Rotate stack a / b upward (first becomes last) |
| `rr` | `ra` and `rb` at the same time |
| `rra` / `rrb` | Reverse rotate stack a / b (last becomes first) |
| `rrr` | `rra` and `rrb` at the same time |

---

## Instructions

### Compilation

```bash
make        # builds push_swap
make bonus  # builds checker (bonus)
make clean  # removes object files
make fclean # removes object files and binaries
make re     # fclean + all
```

### Usage

```bash
./push_swap [--simple | --medium | --complex | --adaptive] <integers...>
```

| Flag | Strategy |
|------|----------|
| `--simple` | Forces O(n²) algorithm |
| `--medium` | Forces O(n√n) algorithm |
| `--complex` | Forces O(n log n) algorithm |
| `--adaptive` | Selects strategy based on disorder (default) |

Add `--bench` to print sorting metrics to stderr:

```bash
./push_swap --bench --adaptive 4 67 3 87 23
```

The benchmark output includes: disorder percentage, strategy used, total operation count, and per-operation breakdown.

### Examples

```bash
# Basic sort
./push_swap 2 1 3 6 5 8

# Count operations
ARG="4 67 3 87 23"; ./push_swap --adaptive $ARG | wc -l

# Verify correctness with checker (bonus)
ARG="4 67 3 87 23"; ./push_swap --complex $ARG | ./checker $ARG

# Large input benchmark
shuf -i 0-9999 -n 500 > args.txt
./push_swap $(cat args.txt) | wc -l
```

### Error handling

The program prints `Error` to stderr and exits on: non-integer arguments, values outside the int range, and duplicate values.

```bash
./push_swap 0 one 2 3   # Error
./push_swap 3 2 3       # Error
```

### Checker (bonus)

```bash
# Read operations from stdin, print OK or KO
./push_swap 3 2 1 0 | ./checker 3 2 1 0
```

---

## Performance Targets

| Input size | Pass | Good | Excellent |
|------------|------|------|-----------|
| 100 numbers | < 2000 ops | < 1500 ops | < 700 ops |
| 500 numbers | < 12000 ops | < 8000 ops | < 5500 ops |

---

## Nodes and stacks
We defined a node data structure as
```c
typedef struct s_node
{
	int				value;
	int				rank;
	struct s_node	*next;
}	
```
and our stack data structure as
```c
typedef struct s_stack
{
	t_node			*top;
	t_node			*bottom;
	int				size;
}
```
The rank of a node is its distance from the node of smallest value when the nodes are sorted in increasing order. The rank of every node is computed as part of the preprocessing.

## Input

The input is given from the command line and must consist in a series of numbers with no repetitions. The program immediately converts it into a stack.

Before any moves are made, the program computes a disorder score between `0` (already sorted) and `1` (worst-case reverse order). It counts the number of inverted pairs divided by the total number of pairs:

```
disorder = inversions / (n * (n - 1) / 2)
```

This value drives strategy selection in adaptive mode.

## Algorithm Explanation

- **Simple algorithm — O(n²)**
We provided two functions sort_2 and sort_3 that sort stacks with two and three elements respectively, and a function sort_inverted specialized to stacks of distorder 100\%. For the general case, we chose to apply selection sort

- **Medium algorithm — O(n√n)**
We chose to apply a chunk sort. The algorithm inspects the stack *a* and construct the *k*-th chunk by pushing to the stack *b* all the elements with values between *k√n* and *(k+1)√n*. Then, all elements are pushed back to *a* and sorted by a selection sort.

- **Complex algorithm — O(n log n)**
The complex algorithm is a radix sort.

### Adaptive algorithm

Thresholds used and rationale:

| Disorder and size ranges | Strategy used | Complexity |
|----------------|---------------|------------|
| *d = 1.00* | Inverted sort | O(n) |
| *n < 0.95* | Selection sort | O(n²) |
| *95 ≤ n ≤ 150 and d ≤ 0.15 or d ≥ 0.85* | Selection sort | O(n²) |
| *95 ≤ n ≤ 150 and 0.15 < d < 0.85* | Chunk sort | O(n√n) |
| *150 < n ≤ 200* and d ≤ 0.10 or d ≥ 0.90* | Selection sort | O(n²) |
| *150 < n ≤ 200* and 0.10 < d < 0.90* | Chunk sort | O(n√n) |
| *200 < n ≤ 300* and d ≤ 0.05 or d ≥ 0.95* | Chunk sort | O(n√n) |
| All other cases | Radix sort | O(n log n) |

---

## Resources

### References

- [Sorting algorithms — Wikipedia](https://en.wikipedia.org/wiki/Sorting_algorithm)
- [Big-O Cheat Sheet](https://www.bigocheatsheet.com/)
- [Radix sort](https://en.wikipedia.org/wiki/Radix_sort)
- [Merge sort](https://en.wikipedia.org/wiki/Merge_sort)
- [Insertion sort](https://en.wikipedia.org/wiki/Insertion_sort)
- Bhargava, A. — *Grokking Algorithms* — accessible introduction to sorting and complexity


### Testing tools

- [anachiossi/push_swap_toolkit](https://github.com/anachiossi/push_swap_toolkit) — custom testing toolkit for this project (input generator, benchmark runner, performance plots)

### AI usage

Claude (claude.ai) was used during the development of this project for the following purposes:

- **Explaining sorting algorithms** 
- **Testing** — Claude helped testing the program.
- **README writing** — this README itself was written with the help of Claude, following the structure and requirements provided by the 42 curriculum.

