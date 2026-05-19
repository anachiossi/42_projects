/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_comb2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:19:49 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/13 11:21:26 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex06
** ft_print_comb2
** Allowed functions: write
** Key concepts: Output, Nested loops, Combinations
**
** Description:
** Write a function that displays all possible combinations of two different
** two-digit numbers in ascending order, separated by ", ".
** Output: void
** Rules:
** - Each pair must be in ascending order (i < j), where i and j are in [0, 99]
** - Numbers are printed with two digits (00, 01, ..., 99)
** - Pairs are separated by " " within a pair, and ", " between pairs
** Example: 00 01, 00 02, ..., 98 99
*/

#include <unistd.h>

void	ft_print_comb2(void)
{
	int	i;
	int	j;
	char	tens;
	char	ones;

	i = 0;
	while (i <= 98)
	{
		j = i + 1;
		while (j <= 99)
		{
			tens = (i / 10) + '0';
			ones = (i % 10) + '0';
			write(1, &tens, 1);
			write(1, &ones, 1);
			write(1, " ", 1);
			tens = (j / 10) + '0';
			ones = (j % 10) + '0';
			write(1, &tens, 1);
			write(1, &ones, 1);
			if (i != 98 || j != 99)
			{
				write(1, ",", 1);
				write(1, " ", 1);
			}
			j++;
		}
		i++;
	}
}
/*
int	main(void)
{
	ft_print_comb2();
	return (0);
}*/
