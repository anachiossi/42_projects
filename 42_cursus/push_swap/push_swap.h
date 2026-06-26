/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:29:22 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/26 15:02:21 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include "libft/libft.h"
# include <stdlib.h>
# include <unistd.h>

int					ft_printf(const char *format, ...);

typedef struct s_node
{
	int				value;
	int				rank;
	struct s_node	*next;
}					t_node;

typedef struct s_stack
{
	t_node			*top;
	t_node			*bottom;
	int				size;
}					t_stack;

// level if 0 == adaptive, 1 == simple, 2 == medium, 3 == complex
// bench is boolean 1 if --bench is set, 0 otherwise
typedef struct s_ps_flags
{
	int				level;
	int				bench;
}					t_ps_flags;

// parsing — parse.c (ft_atoi from libft)
int					is_valid(char *s);
int					input_to_stack(char **arg, t_stack *a);
int					assign_flags(t_ps_flags *f, char **argv, int argc);
void				assign_ranks(t_stack *a);

// stack creation — stack.c
t_node				*new_node(int value);
t_stack				*new_stack(void);
void				free_stack(t_stack *stack);
void				print_stack(t_stack *stack);

// stack manipulation — stack_utils.c
void				push(t_stack *stack, t_node *new);
t_node				*pop(t_stack *stack);
void				push_bottom(t_stack *stack, t_node *new);
t_node				*pop_bottom(t_stack *stack);

// base operations — operations_base.c
int					*op_count(int k);
void				p(t_stack *x, t_stack *y);
void				r(t_stack *x);
void				r_r(t_stack *x);
void				s(t_stack *x);

// push swap operations — operations_ps.c
void				sa(t_stack *a);
void				sb(t_stack *b);
void				ss(t_stack *a, t_stack *b);
void				pa(t_stack *a, t_stack *b);
void				pb(t_stack *a, t_stack *b);

// rotate operations — operations_r.c
void				ra(t_stack *a);
void				rb(t_stack *b);
void				rr(t_stack *a, t_stack *b);

// reverse rotate operations — operations_rr.c
void				rra(t_stack *a);
void				rrb(t_stack *b);
void				rrr(t_stack *a, t_stack *b);

// adaptive algorithm & flag — dispatcher.c
int					is_sorted(t_stack *a);
float				disorder(t_stack *h);
void				dispatcher(t_stack *a, t_stack *b, t_ps_flags f, float d);
void				adaptive_sort(t_stack *a, t_stack *b,
						t_ps_flags f, float d);

// sorting simple - sort_selection.c
void				sort_2(t_stack *a);
void				sort_3(t_stack *a);
void				sort_inverted(t_stack *a, t_stack *b);
void				selection_sort(t_stack *a, t_stack *b);

// sorting medium - sort_chunk.c
void				chunk_sort(t_stack *a, t_stack *b);

// sorting complex - sort_radix.c
void				radix_sort(t_stack *a, t_stack *b);

// sorting utils - sort_utils.c
void				rank_to_top(t_stack *a, int i);
int					find_max(t_stack *x);
void				max_to_top(t_stack *b);

//benchmode.c
void				benchmode(float d, int *n, int level);

#endif
