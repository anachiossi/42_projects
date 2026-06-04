/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:40:11 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/30 14:39:12 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_count(char c)
{
	int	ret_written;

	ret_written = write(1, &c, 1);
	if (ret_written == -1)
		return (-1);
	return (ret_written);
}

int	ft_putchar_pad(int len, char c)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (ft_putchar_count(c) == -1)
			return (-1);
		i++;
	}
	return (len);
}

int	ft_print_str(char *s, t_flags info)
{
	int	len;
	int	padlen;
	int	ret_pad;
	int	ret_write;

	if (s == NULL)
		return (write(1, "(null)", 6));
	len = (int)ft_strlen(s);
	if (info.precision >= 0 && info.precision < len)
		len = info.precision;
	padlen = info.width - len;
	if (padlen <= 0)
		return (write(1, s, len));
	if (info.left)
	{
		ret_write = write(1, s, len);
		ret_pad = ft_putchar_pad(padlen, ' ');
		return (ft_return_check(ret_pad, ret_write));
	}
	ret_pad = ft_putchar_pad(padlen, ' ');
	ret_write = write(1, s, len);
	return (ft_return_check(ret_pad, ret_write));
}

int	ft_print_char(int c, t_flags info)
{
	int	ret_write;
	int	ret_pad;

	if (info.width > 0)
	{
		if (info.left)
		{
			ret_write = ft_putchar_count(c);
			ret_pad = ft_putchar_pad(info.width - 1, ' ');
		}
		else
		{
			ret_pad = ft_putchar_pad(info.width - 1, info.padchar);
			ret_write = ft_putchar_count(c);
		}
		return (ft_return_check(ret_pad, ret_write));
	}
	return (ft_putchar_count(c));
}

int ft_print_custom (t_flags info)
{
	char * custom;

	custom = "Ana";
	return (ft_print_str(custom, info));
}
