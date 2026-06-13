/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_float.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 13:55:10 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/13 13:55:19 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_float_round(double f, int prec, long *ip, long *fp)
{
	double	mult;
	int		i;

	mult = 1.0;
	i = 0;
	while (i++ < prec)
		mult *= 10;
	*ip = (long)f;
	*fp = (long)((f - *ip) * mult + 0.5);
	if (*fp >= (long)mult)
	{
		(*ip)++;
		*fp = 0;
	}
}

int	ft_float_frac(long frac, int precision)
{
	char	buf[20];
	int		i;

	i = precision;
	while (i > 0)
	{
		buf[i - 1] = '0' + (int)(frac % 10);
		frac /= 10;
		i--;
	}
	if (write(1, ".", 1) == -1)
		return (-1);
	return (write(1, buf, precision) + 1);
}

int	ft_print_float(double f, t_flags info)
{
	int		prec;
	int		is_neg;
	long	ipart;
	long	frac;
	int		ret;

	prec = 6;
	if (info.precision >= 0)
		prec = info.precision;
	is_neg = (f < 0.0);
	if (is_neg)
		f = -f;
	ft_float_round(f, prec, &ipart, &frac);
	if (is_neg)
		ret = ft_putchar_count('-');
	else
		ret = ft_apply_sign(1, info);
	if (ret == -1)
		return (-1);
	ret += ft_print_nbr_base((unsigned long)ipart, "0123456789",
			ft_nbrlen_base((unsigned long)ipart, "0123456789"));
	if (prec > 0)
		ret += ft_float_frac(frac, prec);
	return (ret);
}
