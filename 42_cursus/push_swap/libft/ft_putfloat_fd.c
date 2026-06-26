/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putfloat_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 12:21:40 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/18 12:21:40 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_putfrac_fd(long frac, int prec, int fd)
{
	char	buf[20];
	int		i;

	i = prec;
	while (i-- > 0)
	{
		buf[i] = (char)('0' + frac % 10);
		frac /= 10;
	}
	ft_putchar_fd('.', fd);
	i = 0;
	while (i < prec)
		ft_putchar_fd(buf[i++], fd);
}

void	ft_putfloat_fd(double n, int prec, int fd)
{
	long	ip;
	long	fp;

	if (n < 0.0)
	{
		ft_putchar_fd('-', fd);
		n = -n;
	}
	ft_float_round(n, prec, &ip, &fp);
	ft_putnbr_fd((int)ip, fd);
	if (prec > 0)
		ft_putfrac_fd(fp, prec, fd);
}
