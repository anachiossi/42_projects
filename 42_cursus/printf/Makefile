NAME	= libftprintf.a
CC		= cc
CFLAGS	= -Wall -Wextra -Werror

SRCS	=	ft_printf.c \
			ft_printf_flags.c \
			ft_printf_numbers.c \
			ft_printf_parsers.c \
			ft_printf_pointers.c \
			ft_printf_strings.c \
			ft_printf_float.c

OBJS	= $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: all

.PHONY: all clean fclean re bonus
