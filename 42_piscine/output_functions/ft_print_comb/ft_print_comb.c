/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_comb.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:19:49 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/13 11:21:26 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex05
** ft_print_comb
** Allowed functions: write
** Key concepts: Output, Nested loops, Combinations
**
** Description:
** Write a function that displays all possible combinations of three different
** digits in ascending order, separated by ", ".
** Output: void
** Rules:
** - Each group of three digits must be in ascending order (i < j < k)
** - Groups are separated by ", " except the last one
** Example: 012, 013, 014, ..., 789
*/

#include <unistd.h>

void ft_print_comb (void)
{
	int i;
	int j;
	int k;
	char c;

	i = 0;
	while (i <= 7)
	{
		j = i + 1;
		while (j <= 8)
		{
			k = j + 1;
			while (k <= 9)
			{
				c = i + '0';
				write(1, &c, 1);
				c = j + '0';
				write(1, &c, 1);
				c = k + '0';
				write(1, &c, 1);
				if (i != 7 || j != 8 || k != 9)
					write(1, ", ", 2);
				k++;
			}
			j++;
		}
		i++;
	}
}
/*
int	main(void)
{
	ft_print_comb();
	return (0);
}*/
