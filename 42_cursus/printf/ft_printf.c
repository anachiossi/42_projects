/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 12:13:05 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/02 16:41:09 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_return_check(int ret_pad, int ret_write)
{
	if (ret_pad == -1 || ret_write == -1)
		return (-1);
	return (ret_pad + ret_write);
}

int	ft_handle_conversion(const char *format, int *i, va_list ap)
{
	int		ret_written;
	t_flags	info;

	info = ft_printf_parser(format, i);
	ret_written = ft_dispatch(format[*i], info, ap);
	if (ret_written == -1)
		return (-1);
	return (ret_written);
}

int	ft_handle_text(const char *format, int i)
{
	return (write(1, &format[i], 1));
}

int	ft_printf(const char *format, ...)
{
	int		total;
	int		ret_written;
	va_list	ap;
	int		i;

	i = 0;
	total = 0;
	if (format == NULL)
		return (-1);
	va_start(ap, format);
	while (format[i])
	{
		if (format[i] != '%')
			ret_written = ft_handle_text(format, i);
		else
		{
			i++;
			ret_written = ft_handle_conversion(format, &i, ap);
		}
		if (ret_written == -1)
			return (va_end(ap), -1);
		total += ret_written;
		i++;
	}
	return (va_end(ap), total);
}

int main (void)
{
	ft_printf("%a\n");
	return (0);
}
