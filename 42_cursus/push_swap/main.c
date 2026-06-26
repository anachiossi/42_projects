/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo <marvin@42.fr>>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 18:45:55 by gsantoni          #+#    #+#             */
/*   Updated: 2026/06/26 15:14:25 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	main(int argc, char **argv)
{
	t_stack		*a;
	t_stack		*b;
	t_ps_flags	f;
	float		d;
	int			i;

	if (argc < 2)
		return (0);
	a = new_stack();
	b = new_stack();
	i = assign_flags(&f, argv, argc);
	if (!a || !b || i < 0 || !input_to_stack(&argv[i], a))
		return (free_stack(a), free_stack(b), write(2, "Error\n", 6), 0);
	assign_ranks(a);
	d = disorder(a);
	dispatcher(a, b, f, d);
	if (f.bench)
		benchmode(d, op_count(-1), f.level);
	free_stack(a);
	free_stack(b);
	return (0);
}
