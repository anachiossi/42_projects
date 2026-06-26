/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_parsers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 16:48:09 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/17 15:07:49 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_flags	ft_printf_parser(const char *format, int *i)
{
	t_flags	info;

	info.space = 0;
	info.padchar = ' ';
	info.left = 0;
	info.showsign = 0;
	info.width = 0;
	info.prefix = 0;
	info.precision = -1;
	info = ft_parse_flags(format, i, info);
	if (info.left)
		info.padchar = ' ';
	info = ft_parse_width(format, i, info);
	info = ft_parse_precision(format, i, info);
	return (info);
}

t_flags	ft_parse_flags(const char *format, int *i, t_flags info)
{
	while (format[*i] == '-' || format[*i] == '0' || format[*i] == '#'
		|| format[*i] == '+' || format[*i] == ' ')
	{
		if (format[*i] == ' ')
			info.space = 1;
		else if (format[*i] == '0')
			info.padchar = '0';
		else if (format[*i] == '-')
			info.left = 1;
		else if (format[*i] == '+')
			info.showsign = 1;
		else if (format[*i] == '#')
			info.prefix = 1;
		*i = *i + 1;
	}
	return (info);
}

t_flags	ft_parse_width(const char *format, int *i, t_flags info)
{
	while (format[*i] >= '0' && format[*i] <= '9')
	{
		info.width = (info.width * 10) + (format[*i] - '0');
		*i = *i + 1;
	}
	return (info);
}

t_flags	ft_parse_precision(const char *format, int *i, t_flags info)
{
	if (format[*i] == '.')
	{
		*i = *i + 1;
		info.precision = 0;
		while (format[*i] >= '0' && format[*i] <= '9')
		{
			info.precision = (info.precision * 10) + (format[*i] - '0');
			*i = *i + 1;
		}
	}
	return (info);
}

int	ft_dispatch(char specifier, t_flags info, va_list ap)
{
	if (specifier == 'c')
		return (ft_print_char((va_arg(ap, int)), info));
	else if (specifier == 's')
		return (ft_print_str((va_arg(ap, char *)), info));
	else if (specifier == 'p')
		return (ft_print_ptr((va_arg(ap, void *)), info));
	else if (specifier == 'd' || specifier == 'i')
		return (ft_print_nbr((va_arg(ap, int)), info));
	else if (specifier == 'u')
		return (ft_print_unsigned(va_arg(ap, unsigned int), info));
	else if (specifier == 'x')
		return (ft_print_hex(va_arg(ap, unsigned int), info, 0));
	else if (specifier == 'X')
		return (ft_print_hex(va_arg(ap, unsigned int), info, 1));
	else if (specifier == 'f')
		return (ft_print_float(va_arg(ap, double), info));
	else if (specifier == '%')
		return (ft_print_char('%', info));
	return (0);
}
