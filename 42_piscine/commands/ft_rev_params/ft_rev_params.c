/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:36:39 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/26 11:21:46 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C06 - ex02
** ft_rev_params
** Allowed functions: write
** Key concepts: argc/argv loop in reverse, decrement loop control
** Description: Display all command-line arguments except argv[0],
**              each on a new line in REVERSE order.
** Example: ./a.out a b c → outputs: c\n b\n a\n
*/

#include <unistd.h>

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	i = argc - 1;
	if (argc < 1)
		return (0);
	while (i != 0)
	{
		j = 0;
		while (argv[i][j])
		{
			write(1, &argv[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i--;
	}
	return (0);
}	
