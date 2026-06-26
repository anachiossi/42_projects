/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_rr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 18:13:05 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/26 14:57:06 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_stack *a)
{
	r_r(a);
	op_count(8);
}

void	rrb(t_stack *b)
{
	r_r(b);
	op_count(9);
}

void	rrr(t_stack *a, t_stack *b)
{
	r_r(a);
	r_r(b);
	op_count(10);
}
