*This project has been created as part of the 42 curriculum by anade-mo.*

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
| `ft_isalpha` | Takes a character. Checks if it is alphabetic. Returns 1 if true, 0 otherwise. |
| `ft_isdigit` | Takes a character. Checks if it is a digit. Returns 1 if true, 0 otherwise. |
| `ft_isalnum` | Takes a character. Checks if it is alphanumeric. Returns 1 if true, 0 otherwise. |
| `ft_isascii` | Takes a character. Checks if it is in the ASCII range (0–127). Returns 1 if true, 0 otherwise. |
| `ft_isprint` | Takes a character. Checks if it is printable. Returns 1 if true, 0 otherwise. |
| `ft_strlen` | Takes a string. Counts its characters until the null terminator. Returns the length as an int. |
| `ft_memset` | Takes a memory area, a byte value and a size. Fills n bytes with that value. Returns a pointer to the memory area. |
| `ft_bzero` | Takes a memory area and a size. Sets n bytes to zero. Returns nothing. |
| `ft_memcpy` | Takes a destination, a source and a size. Copies n bytes from src to dst without handling overlaps. Returns a pointer to dst. |
| `ft_memmove` | Takes a destination, a source and a size. Copies n bytes from src to dst handling overlapping memory. Returns a pointer to dst. |
| `ft_strlcpy` | Takes a destination buffer, a source string and a size. Copies the string into the buffer up to size. Returns the length of src. |
| `ft_strlcat` | Takes a destination buffer, a source string and a size. Appends src to dst up to size. Returns the total length it tried to create. |
| `ft_toupper` | Takes a character. Converts it to uppercase if it is a lowercase letter. Returns the converted character. |
| `ft_tolower` | Takes a character. Converts it to lowercase if it is an uppercase letter. Returns the converted character. |
| `ft_strchr` | Takes a string and a character. Searches for the first occurrence of that character. Returns a pointer to it, or NULL if not found. |
| `ft_strrchr` | Takes a string and a character. Searches for the last occurrence of that character. Returns a pointer to it, or NULL if not found. |
| `ft_strncmp` | Takes two strings and a size. Compares up to n characters of both. Returns 0 if equal, positive or negative otherwise. |
| `ft_memchr` | Takes a memory area, a character and a size. Searches n bytes for that character. Returns a pointer to it, or NULL if not found. |
| `ft_memcmp` | Takes two memory areas and a size. Compares n bytes of both. Returns 0 if equal, positive or negative otherwise. |
| `ft_strnstr` | Takes a string, a substring and a size. Searches for the substring within at most n characters. Returns a pointer to it, or NULL if not found. |
| `ft_atoi` | Takes a string. Converts it to its integer representation. Returns the integer. |
| `ft_calloc` | Takes a number of elements and a size. Allocates zero-initialised memory for that array. Returns a pointer to the allocated memory. |
| `ft_strdup` | Takes a string. Creates a duplicate of it. Returns a malloc'd copy. |

### Part 2 — Additional functions

Utility functions not present in the standard libc.

| Function | Description |
|---|---|
| `ft_substr` | Takes a string, a starting index and a len. Extracts that part of the string from index to len. Returns a malloc'd substring. |
| `ft_strjoin` | Takes two strings. Concatenates them into a new string. Returns a malloc'd result. |
| `ft_strtrim` | Takes a string and a set of characters. Trims those characters from both ends of the string. Returns a malloc'd copy. |
| `ft_split` | Takes a string and a delimiter character. Splits the string by that delimiter. Returns a malloc'd null-terminated array of strings. |
| `ft_itoa` | Takes an integer. Converts it to its string representation. Returns a malloc'd string. |
| `ft_strmapi` | Takes a string and a function. Applies the function to each character. Returns a new malloc'd string. |
| `ft_striteri` | Takes a string and a function. Applies the function to each character in place. Returns nothing, alters the original string. |
| `ft_putchar_fd` | Takes a character and a file descriptor. Outputs the character to that fd. Returns nothing. |
| `ft_putstr_fd` | Takes a string and a file descriptor. Outputs the string to that fd. Returns nothing. |
| `ft_putendl_fd` | Takes a string and a file descriptor. Outputs the string followed by a newline to that fd. Returns nothing. |
| `ft_putnbr_fd` | Takes an integer and a file descriptor. Outputs the integer to that fd. Returns nothing. |

### Part 3 — Linked list functions

| Function | Description |
|---|---|
| `ft_lstnew` | Takes a *content and creates a new node, return pointer to the new node. Uses malloc |
| `ft_lstadd_front` | Takes the &lst to the first node and a new node, adds a node at the beginning of a list. Void return. Alters the list itself|
| `ft_lstsize` | Takes the pointer of (ideally) the head of the list and counts the nodes. Returns the number of nodes in a list. |
| `ft_lstlast` | Takes any pointer of the list, goes to next node until last. Return the last node of the list.|
| `ft_lstadd_back` | Takes the &lst to any node of the list, goes by every node until last, and adds a node at the end of a list. Void return. Alters the list itself |
| `ft_lstdelone` | Takes the node to be free and the pointer to a function to delete it. Frees a node's content and the node itself, but dont free the list. |
| `ft_lstclear` | Takes &lst of a pointer of (ideally) the head node of a list, and the pointer to a function to delete it. Deletes and frees all nodes of a list from that point on.|
| `ft_lstiter` | Takes the &lst of a pointer to a node (idealy head), and the pointer to a any function (idealy one that can be applied to the node content). Applies a function to the content of each node, changing the list's content's node forever|
| `ft_lstmap` | Takes the &lst of a pointer to a node (idealy head), the pointer of a function, and a pointer to a delete function. Use malloc to create a new list, same size of original. Applies a function to each node's content and returns the new list pointer, without changing original list |

## Resources

### References

- [Man pages](https://man7.org/linux/man-pages/) — primary reference for all function behaviors

### Books

- K.N. King — *C Programming: A Modern Approach* — Ch. 8, 11, 13, 15, 17
- Bhargava — *Grokking Algorithms* — Ch. 2, 3
- Hunt & Thomas — *The Pragmatic Programmer* — Ch. 1
- Bryant & O'Hallaron — *Computer Systems: A Programmer's Perspective* — Ch. 2, 9

### AI usage

AI was used as a support tool for organization (using app Notion), planning, concept clarification and README drafts. It also helped me to write a tester in C for all functions, in order to me to learn how to directly apply the functions and how to use valdrid to test memory leaks. 

---
