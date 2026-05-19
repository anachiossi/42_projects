/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/13 11:19:49 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/13 11:21:26 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C00 - ex07
** ft_putnbr
** Allowed functions: write
** Key concepts: Output, Recursion, INT_MIN edge case
**
** Description:
** Write a function that displays an integer to stdout.
** Handles all integers including INT_MIN (-2147483648).
** Input: int nb (the number to display)
** Output: void
** Example: ft_putnbr(42) → 42, ft_putnbr(-2147483648) → -2147483648
*/

#include <unistd.h>

void	ft_putnbr(int nb)
{
	char	c;

	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (nb < 0)
	{
		write(1, "-", 1);
		nb = -nb;
	}
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
	}
	c = '0' + (nb % 10);
	write(1, &c, 1);
}
/*
int	main(void)
{
	ft_putnbr(42);
	ft_putnbr(-217483648);
	ft_putnbr(-42);
	ft_putnbr(88888888);
	return (0);
}
*/
