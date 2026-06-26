/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_numbers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 15:38:07 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/30 14:42:23 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbrlen_base(unsigned long nbr, char *base)
{
	int	nbrlen;
	int	baselen;

	baselen = (int)ft_strlen(base);
	nbrlen = 0;
	if (nbr == 0)
		return (1);
	while (nbr > 0)
	{
		nbr = nbr / baselen;
		nbrlen++;
	}
	return (nbrlen);
}

int	ft_print_nbr_base(unsigned long nbr, char *base, int nbrlen)
{
	char	output[20];
	int		i;
	int		baselen;

	if (nbrlen == 0)
		return (0);
	baselen = (int)ft_strlen(base);
	i = 19;
	if (nbr == 0)
	{
		output[0] = base[nbr % baselen];
		return (write(1, &output[0], 1));
	}
	while (nbr > 0)
	{
		output[i] = base[nbr % baselen];
		nbr = nbr / baselen;
		i--;
	}
	return (write(1, &output[i + 1], 19 - i));
}

int	ft_print_nbr_content(int n, int nbrlen, int precise_len, t_flags info)
{
	unsigned long	nbr;
	int				ret_sign;
	int				ret_precision;
	int				ret_digits;
	int				ret_pad;

	ret_pad = 0;
	nbr = (unsigned long)n;
	if (n < 0)
		nbr = -(unsigned long)n;
	ret_sign = ft_apply_sign((long)n, info);
	if (!info.left && info.padchar == '0')
		ret_pad = ft_apply_padding((precise_len), info);
	if (ret_sign == -1 || ret_pad == -1)
		return (-1);
	ret_precision = ft_apply_precision(nbrlen, info);
	if (nbrlen > 0)
		ret_digits = ft_print_nbr_base(nbr, "0123456789", nbrlen);
	else
		ret_digits = 0;
	if (ret_precision == -1 || ret_digits == -1)
		return (-1);
	return (ret_digits + ret_precision + ret_pad + ret_sign);
}

int	ft_print_nbr(int n, t_flags info)
{
	unsigned long	nbr;
	int				nbrlen;
	int				precise_len;
	int				ret_pad;
	int				ret_content;

	nbrlen = 0;
	ret_pad = 0;
	ret_content = 0;
	nbr = (unsigned long)n;
	if (n < 0)
		nbr = -(unsigned long)n;
	precise_len = ft_calc_precise_len(nbr, "0123456789", info, &nbrlen);
	if (n < 0 || info.showsign || info.space)
		precise_len++;
	if (!info.left && info.padchar != '0')
		ret_pad = ft_apply_padding(precise_len, info);
	ret_content = ft_print_nbr_content(n, nbrlen, precise_len, info);
	if (info.left)
		ret_pad = ft_apply_padding(precise_len, info);
	return (ft_return_check(ret_pad, ret_content));
}

int	ft_print_unsigned(unsigned int nbr, t_flags info)
{
	int	nbrlen;
	int	precise_len;
	int	ret_pad;
	int	ret_precision;
	int	ret_digits;

	precise_len = ft_calc_precise_len((unsigned long)nbr, "0123456789",
			info, &nbrlen);
	if (info.left)
	{
		ret_precision = ft_apply_precision(nbrlen, info);
		ret_digits = ft_print_nbr_base((unsigned long)nbr, "0123456789",
				nbrlen);
		ret_pad = ft_apply_padding(precise_len, info);
		return (ft_return_check(ret_pad, ft_return_check(ret_digits,
					ret_precision)));
	}
	ret_pad = ft_apply_padding(precise_len, info);
	ret_precision = ft_apply_precision(nbrlen, info);
	ret_digits = ft_print_nbr_base((unsigned long)nbr, "0123456789", nbrlen);
	return (ft_return_check(ret_digits,
			ft_return_check(ret_precision, ret_pad)));
}
