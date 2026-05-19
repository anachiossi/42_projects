/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/11 11:58:12 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/12 07:51:48 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex00
** ft_putchar
** Allowed functions: write
** Key concepts: Output, System calls
**
** Description:
** Write a function that outputs a character to stdout.
** Input: char c (the character to display)
** Output: void
*/

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}
