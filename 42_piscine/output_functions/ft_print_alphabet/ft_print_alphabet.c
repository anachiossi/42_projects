/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_alphabet.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 08:18:06 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/13 18:32:12 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex01
** ft_print_alphabet
** Allowed functions: write
** Key concepts: Output, Loops, ASCII
**
** Description:
** Write a function that displays the lowercase alphabet on a single line,
** in alphabetical order, starting from 'a'.
** Output: void
** Example: abcdefghijklmnopqrstuvwxyz
*/

#include <unistd.h>

void	ft_print_alphabet(void)
{
	char	c;

	c = 'a';
	while (c <= 'z')
	{
		write (1, &c, 1);
		c = c + 1;
	}
}
