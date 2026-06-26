/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_selection.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 19:48:54 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/26 15:00:31 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_2(t_stack *a)
{
	if (a->top->value > a->top->next->value)
		sa(a);
}

void	sort_3(t_stack *a)
{
	int	top;
	int	mid;
	int	bot;

	top = a->top->value;
	mid = a->top->next->value;
	bot = a->bottom->value;
	if (top < mid && mid < bot)
		return ;
	if (top > mid && top > bot && mid < bot)
		ra(bench, a);
	else if (top > mid && top > bot && mid > bot)
	{
		sa(a);
		rra(a);
	}
	else if (top < mid && top < bot)
	{
		rra(a);
		sa(a);
	}
	else if (top > mid)
		sa(a);
	else
		rra(a);
}

void	sort_inverted(t_stack *a, t_stack *b)
{
	int	n;

	if (a->size == 2)
		return (sort_2(a));
	if (a->size == 3)
		return (sort_3(a));
	n = a->size - 4;
	sa(a);
	while (n > 0)
	{
		rra(a);
		pb(a, b);
		n--;
	}
	ra(a);
	ra(a);
	sa(a);
	while (b->size > 0)
		pa(a, b);
}

void	selection_sort(t_stack *a, t_stack *b)
{
	int	i;

	i = 0;
	while (a->size > 3 && !is_sorted(a))
	{
		rank_to_top(a, i);
		pb(a, b);
		i++;
	}
	if (a->size == 2)
		sort_2(a);
	else if (a->size == 3)
		sort_3(a);
	while (b->size)
		pa(a, b);
}
