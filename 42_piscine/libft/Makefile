NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = srcs/ft_putchar.c srcs/ft_swap.c srcs/ft_putstr.c srcs/ft_strlen.c srcs/ft_strcmp.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)
$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c srcs/$(@F:.o=.c) -o $@
clean:
	rm -f $(OBJS)
fclean: clean
	rm -f $(NAME)
re: fclean all

