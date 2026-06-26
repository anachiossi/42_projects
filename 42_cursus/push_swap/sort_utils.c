/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 19:48:54 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/26 15:00:54 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rank_to_top(t_stack *a, int i)
{
	int		pos;
	t_node	*cur;

	pos = 0;
	cur = a->top;
	while (cur->rank != i)
	{
		pos++;
		cur = cur->next;
	}
	if (pos <= a->size / 2)
		while (a->top->rank != i)
			ra(a);
	else
		while (a->top->rank != i)
			rra(a);
}

int	find_max(t_stack *x)
{
	int		max;
	t_node	*current;

	max = x->top->value;
	current = x->top->next;
	while (current)
	{
		if (current->value > max)
			max = current->value;
		current = current->next;
	}
	return (max);
}

void	max_to_top(t_stack *b)
{
	int		max;
	int		pos;
	t_node	*cur;

	max = find_max(b);
	pos = 0;
	cur = b->top;
	while (cur->value != max)
	{
		pos++;
		cur = cur->next;
	}
	if (pos <= b->size / 2)
		while (b->top->value != max)
			rb(b);
	else
		while (b->top->value != max)
			rrb(b);
}
