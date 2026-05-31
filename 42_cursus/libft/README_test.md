# Testing libft

## Compile and run tests

Make sure `libft.a` is built first:
```bash
make
```

Then compile and run the test file:
```bash
cc main.c -L. -lft -lbsd -o test && ./test

```
## Memory Leek Test ##
```bash
valgrind --leak-check=full --show-leak-kinds=all ./test
```
## Norminette excluding main.c
```bash
for f in *.c; do [ "$f" != "main.c" ] && norminette "$f"; done
```
## Files (not submitted)

- `main.c` — test file

