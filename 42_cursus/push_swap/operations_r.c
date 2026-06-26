/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_r.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 13:58:00 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/26 14:56:55 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	ra(t_stack *a)
{
	r(a);
	op_count(5);
	write(1, "ra\n", 3);
}

void	rb(t_stack *b)
{
	r(b);
	op_count(6);
	write(1, "rb\n", 3);
}

void	rr(t_stack *a, t_stack *b)
{
	r(a);
	r(b);
	op_count(7);
	write(1, "rr\n", 3);
}
