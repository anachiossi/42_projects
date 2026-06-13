*This project has been created as part of the 42 curriculum by anade-mo.*

## Description 

> 42 Roma Elis — custom reimplementation of `printf(3)`

---

## Folder structure

```
ft_printf/
│
├── ft_printf.c                 ← main loop + va_list lifecycle
├── ft_printf_parsers.c         ← flag/width/precision parser + dispatch
├── ft_printf_strings.c         ← %c and %s handlers
├── ft_printf_numbers.c         ← %d, %i, %u handlers
├── ft_printf_pointers.c        ← %p, %x, %X handlers
├── ft_printf_flags.c           ← padding, sign, prefix helpers
├── ft_printf_float.c           ← %f handler
│
├── ft_printf.h                 ← t_flags struct + all prototypes
├── Makefile                    ← builds libftprintf.a
└── README.md
```

---

## Program flow

```
ft_printf(fmt, ...)
│
├── va_start(ap, fmt)           ← aim cursor at first variadic arg
│
├── walk fmt char by char
│   │
│   ├── literal char            → ft_handle_text → write it, count it
│   │
│   └── '%' found
│       │
│       ├── ft_printf_parsers.c
│       │   ├── ft_parse_flags     { '-', '0', '#', '+', ' ' }
│       │   ├── ft_parse_width     { digits }
│       │   └── ft_parse_precision { '.' then digits }
│       │       → returns t_flags
│       │
│       └── ft_dispatch(spec, flags, ap)
│           │
│           ├── %c  → ft_print_char
│           ├── %s  → ft_print_str
│           ├── %d  → ft_print_nbr
│           ├── %i  → ft_print_nbr
│           ├── %u  → ft_print_unsigned
│           ├── %x  → ft_print_hex(is_upper = false)
│           ├── %X  → ft_print_hex(is_upper = true)
│           ├── %p  → ft_print_ptr
│           ├── %f  → ft_print_float
│           └── %%  → ft_print_char('%')
│               │
│               └── every handler returns:
│                   -1  → write failed → ft_printf returns -1
│                    N  → chars written → add to total
│
├── va_end(ap)                  ← mandatory cleanup before return
│
└── return total                ← total chars written, or -1 on error
```

---

## Instructions

```
make        → builds libftprintf.a
make clean  → removes .o files
make fclean → removes .o files + libftprintf.a
make re     → fclean + make (full rebuild)
make bonus  → same as all
```

---

## The Norm 

42's style enforcer. A norm error anywhere → grade is 0.

- No `for`, no `do-while`, no `switch`, no ternary (`? :`)
- Max 25 lines per function, max 4 parameters
- Max 5 functions per `.c` file
- `return (value);` — parentheses are mandatory

---

## Testing with Tripouille's printfTester

### Setup

```bash
cd ft_printf/
git clone https://github.com/Tripouille/printfTester printfTester
cd printfTester
```

### Running tests

```bash
make m          # all mandatory
make c          # %c only
make s          # %s only
make p          # %p only
make d          # %d only
make i          # %i only
make u          # %u only
make x          # %x only
make upperx     # %X only
make percent    # %% only
make mix        # mixed

# bonus tests (flags)
make minus      # '-' flag
make 0          # '0' flag
make dot        # '.' precision
make sharp      # '#' flag
make space      # ' ' flag
make +          # '+' flag
```

---

## Personal notes

My learning curve from learning printf to planning project execution, to documenting architecture decisions:
Disclamer: let me know if you find something conceptually wrong. 

https://square-green-2ba.notion.site/Printf-36c82782872c809c9e0add6f68fffbbb

---

## Resources

- `man 3 printf`
- `man 3 stdarg`
- musl libc source: https://git.musl-libc.org/cgit/musl/tree/src/stdio/vfprintf.c
- glibc source: https://sourceware.org/git/?p=glibc.git;a=blob;f=stdio-common/vfprintf-internal.c
- YouTube: https://youtu.be/DYaIgM2XI-E?si=F8KbpcWZ0H600okx
- YouTube: https://youtu.be/Hb2m7htiKWM?si=wwDqzGoIB354xfXV
- YouTube: https://youtu.be/VYTF4KIF2z0?si=a13tlTDrTob_z31l

---

### AI usage

AI was used as a support tool for organization (using app Notion), planning, concept clarification, code review and README drafts.
