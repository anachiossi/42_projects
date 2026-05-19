#!/bin/bash
cc -c -Wall -Werror -Wextra ft_putchar.c
cc -c -Wall -Werror -Wextra ft_swap.c
cc -c -Wall -Werror -Wextra ft_putstr.c
cc -c -Wall -Werror -Wextra ft_strlen.c
cc -c -Wall -Werror -Wextra ft_strcmp.c
ar rc libft.a ft_putchar.o ft_swap.o ft_putstr.o ft_strlen.o ft_strcmp.o

