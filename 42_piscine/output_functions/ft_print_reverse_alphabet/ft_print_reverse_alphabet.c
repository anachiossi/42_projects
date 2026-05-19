/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_reverse_alphabet.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 10:20:52 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/12 10:27:13 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex02
** ft_print_reverse_alphabet
** Allowed functions: write
** Key concepts: Output, Loops, ASCII
**
** Description:
** Write a function that displays the lowercase alphabet on a single line,
** in reverse order, starting from 'z'.
** Output: void
** Example: zyxwvutsrqponmlkjihgfedcba
*/

#include <unistd.h>

void	ft_print_reverse_alphabet(void)
{
	char	c;

	c = 'z';
	while (c >= 'a')
	{
		write (1, &c, 1);
		c = c - 1;
	}
}
