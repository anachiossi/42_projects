/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_params.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:35:58 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/26 11:20:34 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C06 - ex01
** ft_print_params
** Allowed functions: write
** Key concepts: argc/argv loop, argument iteration, skipping argv[0]
** Description: Display all command-line arguments except argv[0],
**              each on a new line in order.
** Example: ./a.out test1 test2 test3 → outputs: test1\ntest2\ntest3\n
*/

#include <unistd.h>

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	j = 1;
	if (argc == 1)
		return (1);
	while (argv[j]) 
	{
		i = 0;
		while (argv[j][i])
		{
			write(1, &argv[j][i], 1);
			i++;
		}
		write(1, "\n", 1);
		j++;
	}
	return (0);
}
