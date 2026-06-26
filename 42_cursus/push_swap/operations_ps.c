/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_ps.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 10:29:16 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/26 14:55:45 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sa(t_stack *a)
{
	s(a);
	op_count(0);
}

void	sb(t_stack *b)
{
	s(b);
	op_count(1);
}

void	ss(t_stack *a, t_stack *b)
{
	s(a);
	s(b);
	op_count(2);
}

void	pa(t_stack *a, t_stack *b)
{
	p(b, a);
	op_count(3);
}

void	pb(t_stack *a, t_stack *b)
{
	p(a, b);
	op_count(4);
}
