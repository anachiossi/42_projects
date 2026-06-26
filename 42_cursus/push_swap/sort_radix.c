/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_radix.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 15:59:56 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/26 14:59:02 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	radix_iter(unsigned int n, t_stack *a, t_stack *b)
{
	int	size;
	int	i;

	size = a->size;
	i = 0;
	while (i < size)
	{
		if (!(((unsigned int)a->top->rank >> n) & 1))
			pb(a, b);
		else
			ra(a);
		i++;
	}
	while (b->size)
		pa(a, b);
}

void	radix_sort(t_stack *a, t_stack *b)
{
	unsigned int	n;
	unsigned int	p;
	unsigned int	digits;

	if (a->size <= 1)
		return ;
	digits = 0;
	p = (unsigned int)(a->size - 1);
	while (p)
	{
		p >>= 1;
		digits++;
	}
	n = 0;
	while (n < digits)
		radix_iter(n++, a, b);
}
