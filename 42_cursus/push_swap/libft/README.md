*This project has been created as part of the 42 curriculum by [yourlogin].*

# libft

## Description

Libft is a C library built from scratch as part of the 42 curriculum. It reimplements a set of standard libc functions, adds extra utility functions not found in the libc, and introduces linked list manipulation. The library is compiled into a static archive `libft.a` that can be reused across future 42 projects.

## Instructions

### Compilation

```bash
make        # builds libft.a
make clean  # removes object files
make fclean # removes object files and libft.a
make re     # fclean + make
```

### Usage

Link the library when compiling your own project:

```bash
cc main.c -L. -lft -o program
```

Include the header in your source files:

```c
#include "libft.h"
```

## Library overview

### Part 1 — Libc functions

Reimplementations of standard libc functions with the `ft_` prefix.

| Function | Description |
|---|---|
| `ft_isalpha` | Returns 1 if character is alphabetic, 0 otherwise |
| `ft_isdigit` | Returns 1 if character is a digit, 0 otherwise |
| `ft_isalnum` | Returns 1 if character is alphanumeric, 0 otherwise |
| `ft_isascii` | Returns 1 if character is in ASCII range (0–127), 0 otherwise |
| `ft_isprint` | Returns 1 if character is printable, 0 otherwise |
| `ft_strlen` | Returns the length of a string |
| `ft_memset` | Fills n bytes of memory with a constant byte |
| `ft_bzero` | Sets n bytes of memory to zero |
| `ft_memcpy` | Copies n bytes from src to dst (no overlap handling) |
| `ft_memmove` | Copies n bytes from src to dst (handles overlapping memory) |
| `ft_strlcpy` | Copies a string into a buffer of a given size |
| `ft_strlcat` | Appends a string to a buffer up to a given size |
| `ft_toupper` | Converts a lowercase letter to uppercase |
| `ft_tolower` | Converts an uppercase letter to lowercase |
| `ft_strchr` | Returns pointer to first occurrence of character in string |
| `ft_strrchr` | Returns pointer to last occurrence of character in string |
| `ft_strncmp` | Compares up to n characters of two strings |
| `ft_memchr` | Searches n bytes of memory for a character |
| `ft_memcmp` | Compares n bytes of two memory areas |
| `ft_strnstr` | Locates a substring within a string, searching at most n characters |
| `ft_atoi` | Converts a string to an integer |
| `ft_calloc` | Allocates zero-initialised memory for an array |
| `ft_strdup` | Returns a malloc'd duplicate of a string |

### Part 2 — Additional functions

Utility functions not present in the standard libc.

| Function | Description |
|---|---|
| `ft_substr` | Returns a malloc'd substring from a string |
| `ft_strjoin` | Returns a malloc'd concatenation of two strings |
| `ft_strtrim` | Returns a malloc'd copy of a string with characters trimmed from both ends |
| `ft_split` | Returns a null-terminated array of strings split by a delimiter |
| `ft_itoa` | Returns a malloc'd string representation of an integer |
| `ft_strmapi` | Applies a function to each character of a string, returning a new string |
| `ft_striteri` | Applies a function to each character of a string in place |
| `ft_putchar_fd` | Outputs a character to a file descriptor |
| `ft_putstr_fd` | Outputs a string to a file descriptor |
| `ft_putendl_fd` | Outputs a string followed by a newline to a file descriptor |
| `ft_putnbr_fd` | Outputs an integer to a file descriptor |

### Part 3 — Linked list functions

Functions to manipulate a singly linked list using the `t_list` struct.
linked lists are a type of struct
type > struct
t_persona > s_persona

| Function | Description |
|---|---|
| `ft_lstnew` | Creates a new list node |
| `ft_lstadd_front` | Adds a node at the beginning of a list |
| `ft_lstsize` | Returns the number of nodes in a list |
| `ft_lstlast` | Returns the last node of a list |
| `ft_lstadd_back` | Adds a node at the end of a list |
| `ft_lstdelone` | Frees a node's content and the node itself |
| `ft_lstclear` | Deletes and frees all nodes of a list |
| `ft_lstiter` | Applies a function to the content of each node |
| `ft_lstmap` | Applies a function to each node and returns a new list |

## Resources

### References

- [Man pages](https://man7.org/linux/man-pages/) — primary reference for all function behaviors
- [GNU C Library documentation](https://www.gnu.org/software/libc/manual/) — deeper reference for libc behavior
- [IBM Documentation](https://www.ibm.com/docs/it/i/7.6.0?topic=functions) — C library function reference

### Books

- K.N. King — *C Programming: A Modern Approach* — Ch. 8, 11, 13, 15, 17
- Bhargava — *Grokking Algorithms* — Ch. 2, 3
- Hunt & Thomas — *The Pragmatic Programmer* — Ch. 1
- Bryant & O'Hallaron — *Computer Systems: A Programmer's Perspective* — Ch. 2, 9

### AI usage

AI was used as a support tool for organization (using app Notion), planning, concept clarification and README drafts. It also helped me to write a tester in C for all functions, in order to me to learn how to directly apply the functions and how to use valdrid to test memory leaks. 

---
