/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flags.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:33:05 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/30 14:36:20 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_apply_precision(int nbrlen, t_flags info)
{
	int	ret_precision;
	int	padlen;

	if (info.precision <= nbrlen)
		return (0);
	padlen = info.precision - nbrlen;
	ret_precision = ft_putchar_pad(padlen, '0');
	return (ret_precision);
}

int	ft_apply_sign(long n, t_flags info)
{
	int	ret_sign;

	if (n < 0)
		ret_sign = ft_putchar_count('-');
	else if (info.showsign)
		ret_sign = ft_putchar_count('+');
	else if (info.space)
		ret_sign = ft_putchar_count(' ');
	else
		return (0);
	return (ret_sign);
}

int	ft_apply_padding(int len, t_flags info)
{
	int	padlen;
	int	ret_pad;

	padlen = info.width - len;
	if (padlen <= 0)
		return (0);
	if (info.left)
		ret_pad = ft_putchar_pad(padlen, ' ');
	else
		ret_pad = ft_putchar_pad(padlen, info.padchar);
	return (ret_pad);
}

int	ft_apply_prefix(int is_upper, int is_pointer, t_flags info)
{
	int	ret_prefix;

	ret_prefix = 0;
	if (info.prefix)
	{
		if (is_upper == 1)
			ret_prefix = write(1, "0X", 2);
		else
			ret_prefix = write(1, "0x", 2);
	}
	if (is_pointer == 1)
		ret_prefix = write(1, "0x", 2);
	return (ret_prefix);
}
