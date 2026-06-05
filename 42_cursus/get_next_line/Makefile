NAME	= get_next_line.a
NAME_BONUS	= get_next_line_bonus.a

OBJS_DIR	= objs

SRCS	= get_next_line.c \
		get_next_line_utils.c

SRCS_BONUS	= get_next_line_bonus.c \
		get_next_line_utils_bonus.c

OBJS	= $(SRCS:%.c=$(OBJS_DIR)/%.o)
OBJS_BONUS	= $(SRCS_BONUS:%.c=$(OBJS_DIR)/%.o)

CC	 = cc
CFLAGS	= -Wall -Wextra -Werror
BUFFER_SIZE	= 42
CPPFLAGS	= -D BUFFER_SIZE=$(BUFFER_SIZE)
AR	= ar rcs
RM	= rm -rf

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
	$(AR) $(NAME_BONUS) $(OBJS_BONUS)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)
clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(NAME) $(NAME_BONUS) tests/karaoke tests/stdin tests/quiz

test: $(NAME) $(NAME_BONUS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -I. get_next_line.c get_next_line_utils.c tests/main_karaoke.c -o tests/karaoke
	$(CC) $(CFLAGS) $(CPPFLAGS) -I. get_next_line.c get_next_line_utils.c tests/main_stdin.c -o tests/stdin
	$(CC) $(CFLAGS) $(CPPFLAGS) -I. get_next_line_bonus.c get_next_line_utils_bonus.c tests/main_quiz.c -o tests/quiz

re: fclean all

.PHONY: all bonus test clean fclean re
