/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_chunk.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 15:47:09 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/26 14:58:32 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	ft_sqrt(int n)
{
	int	i;

	i = 0;
	while ((i + 1) * (i + 1) <= n)
		i++;
	return (i);
}

static void	chunk_sort_b(t_stack *a, t_stack *b)
{
	while (b->size > 0)
	{
		max_to_top(b);
		pa(a, b);
	}
}

void	chunk_sort(t_stack *a, t_stack *b)
{
	int	chunk_size;
	int	chunk_id;
	int	limit;
	int	i;

	chunk_id = 0;
	chunk_size = ft_sqrt(a->size);
	while (a->size > 0)
	{
		limit = (chunk_id + 1) * chunk_size;
		i = a->size;
		while (i > 0 && a->size > 0)
		{
			if (a->top->rank < limit)
				pb(a, b);
			else
			{
				ra(a);
				i--;
			}
		}
		chunk_id++;
	}
	chunk_sort_b(a, b);
}
