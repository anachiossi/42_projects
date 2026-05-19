/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:41:36 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/24 16:42:20 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C04 - ex04
** ft_putnbr_base
** Allowed functions: write
** Key concepts: Output, Bases, Number conversion
**
** Description:
** Write a function that displays an integer in a given base.
** The base is a string containing all valid digit characters.
** Input: int nbr (the number to display), char *base (the base string)
** Output: void
** Rules:
** - If the base is invalid (less than 2 chars, contains '+'/'-', or has
**   duplicate characters), the function does nothing
** - Handles negative numbers
** Example: ft_putnbr_base(255, "0123456789ABCDEF") → FF
*/

#include <unistd.h>

int	ft_base_control(char *base)
{
	int	i;
	int	j;
	int	base_len;

	i = 0;
	base_len = 0;
	while (base[i])
	{
		base_len++;
		if (base[i] == '+' || base[i] == '-')
			return (0);
		j = i + 1;
		while (base[j])
		{
			if (base[j] == base[i])
				return (0);
			j++;
		}
		i++;
	}
	return (base_len);
}

void	print_reverse(char *res, int i)
{
	while (i > 0)
	{
		i--;
		write(1, &res[i], 1);
	}
}

void	print_negative(long *n)
{
	if (*n < 0)
	{
		write(1, "-", 1);
		*n = -*n;
	}
}

void	ft_putnbr_base(int nbr, char *base)
{
	int		base_len;
	char	res[500];
	long	n;
	int		i;

	i = 0;
	n = nbr;
	base_len = ft_base_control(base);
	if (base_len < 2)
		return ;
	if (n == 0)
	{
		write(1, &base[0], 1);
		return ;
	}
	print_negative(&n);
	while (n > 0)
	{
		res[i] = base[n % base_len];
		n = n / base_len;
		i++;
	}
	print_reverse(res, i);
}
