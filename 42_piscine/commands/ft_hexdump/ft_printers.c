/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 16:53:04 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/18 14:33:09 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

void	ft_puthex(int nbr, int digits)
{
	char	*base;

	base = "0123456789abcdef";
	if (digits > 0)
	{
		ft_puthex(nbr / 16, digits - 1);
		write(1, &base[nbr % 16], 1);
	}
	return ;
}

void	ft_char_to_hex(unsigned char *buffer, int bytes)
{
	int	i;

	i = 0;
	write(1, "  ", 2);
	while (i < bytes)
	{
		ft_puthex(buffer[i], 2);
		if (i == 7)
			write(1, "  ", 2);
		else if (i < bytes - 1 || bytes == 16)
			write(1, " ", 1);
		i++;
	}
	if (bytes < 8)
		write(1, "  ", 2);
	while (bytes < 16)
	{
		write(1, "   ", 3);
		bytes++;
	}
	return ;
}

void	ft_print_ascii(unsigned char *buffer, int bytes)
{
	int	i;

	i = 0;
	write(1, " |", 2);
	while (i < bytes)
	{
		if (buffer[i] >= 32 && buffer[i] <= 126)
		{
			write(1, &buffer[i], 1);
		}
		else
		{
			write(1, ".", 1);
		}
		i++;
	}
	write(1, "|\n", 2);
	return ;
}
