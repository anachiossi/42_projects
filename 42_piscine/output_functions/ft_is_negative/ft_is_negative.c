/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_negative.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/12 11:06:47 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/12 11:32:33 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex04
** ft_is_negative
** Allowed functions: write
** Key concepts: Conditionals, Output
**
** Description:
** Write a function that displays 'N' followed by a newline if n is negative,
** or 'P' followed by a newline if n is zero or positive.
** Input: int n (the number to check)
** Output: void
** Example: ft_is_negative(-1) → N, ft_is_negative(0) → P
*/

#include <unistd.h>

void	ft_is_negative(int n)
{
	if (n < 0)
	{
		write (1, "N", 1);
	}
	else
	{
		write (1, "P", 1);
	}
}
/*
int	main(void)
{
	ft_is_negative(-33123123);
	return (0);
}
*/
