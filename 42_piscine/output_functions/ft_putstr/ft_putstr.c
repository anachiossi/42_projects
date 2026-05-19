/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:00:29 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/25 10:46:15 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C04 - ex01
** ft_putstr
** Allowed functions: write
** Key concepts: Output, Strings, Loops
**
** Description:
** Write a function that prints a string of characters to stdout.
** Input: char *str (the string to display)
** Output: void
** Example: ft_putstr("Hello") → Hello
*/
#include <unistd.h>

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		write(1, &str[i], 1);
		i++;
	}
}
