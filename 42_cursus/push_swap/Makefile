NAME	= push_swap
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -I.

LIBFT		= libft/libft.a

SRCS	= main.c parse.c stack.c stack_utils.c \
		  operations_base.c operations_ps.c operations_r.c operations_rr.c \
		  dispatcher.c sort_selection.c sort_chunk.c sort_radix.c sort_utils.c benchmode.c

OBJ_DIR	= obj
OBJS	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

BONUS		= checker
BONUS_DIR	= bonus
BONUS_SRCS	= checker_main_bonus.c checker_bonus.c checker_utils_bonus.c \
			  checker_op_ps_bonus.c checker_op_r_bonus.c checker_op_rr_bonus.c

BONUS_SHARED	= $(filter-out main.c, $(SRCS))
BONUS_OBJS	= $(addprefix $(OBJ_DIR)/, \
			  $(BONUS_SRCS:.c=.o) $(BONUS_SHARED:.c=.o))

LIB			= push_swap.a
LIB_OBJS	= $(addprefix $(OBJ_DIR)/, $(BONUS_SHARED:.c=.o))

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(OBJ_DIR) $(BONUS)

$(BONUS): $(BONUS_OBJS)
	$(MAKE) -C libft
	$(CC) $(CFLAGS) $(BONUS_OBJS) $(LIBFT) -o $(BONUS)

$(LIB): $(OBJ_DIR) $(LIB_OBJS)
	ar rcs $(LIB) $(LIB_OBJS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(BONUS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME) $(BONUS) $(LIB)

re: fclean all

.PHONY: all bonus clean fclean re
