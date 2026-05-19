/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_numbers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 09:53:09 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/12 09:59:18 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex03
** ft_print_numbers
** Allowed functions: write
** Key concepts: Output, Loops, ASCII
**
** Description:
** Write a function that displays the digits 0 through 9 on a single line.
** Output: void
** Example: 0123456789
*/

#include <unistd.h>

void	ft_print_numbers(void)
{
	char	digit;

	digit = '0';
	while (digit <= '9')
	{
		write (1, &digit, 1);
		digit++;
	}
}
