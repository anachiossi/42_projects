/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_pointers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/30 14:40:12 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/30 15:05:09 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_calc_precise_len(unsigned long nbr, char *base,
	t_flags info, int *nbrlen)
{
	int	precise_len;

	*nbrlen = ft_nbrlen_base(nbr, base);
	if (info.precision == 0 && nbr == 0)
		*nbrlen = 0;
	precise_len = *nbrlen;
	if (info.precision > *nbrlen)
		precise_len = info.precision;
	return (precise_len);
}

int	ft_print_ptr_content(unsigned long nbr, int nbrlen,
	t_flags info, int is_pointer)
{
	int	ret_prefix;
	int	ret_precision;
	int	ret_digits;

	ret_prefix = ft_apply_prefix(0, is_pointer, info);
	if (ret_prefix == -1)
		return (-1);
	ret_precision = ft_apply_precision(nbrlen, info);
	if (ret_precision == -1)
		return (-1);
	ret_digits = 0;
	if (nbrlen > 0)
		ret_digits = ft_print_nbr_base(nbr, "0123456789abcdef", nbrlen);
	return (ft_return_check(ret_digits, ret_prefix + ret_precision));
}

int	ft_print_ptr(void *p, t_flags info)
{
	unsigned long	ptr;
	int				nbrlen;
	int				content_len;
	int				ret_pad;
	int				ret_content;

	ptr = (unsigned long)p;
	ret_pad = 0;
	ret_content = 0;
	if (p == NULL)
		return (write(1, "(nil)", 5));
	nbrlen = ft_nbrlen_base(ptr, "0123456789abcdef");
	content_len = nbrlen + 2;
	if (!info.left)
		ret_pad = ft_apply_padding(content_len, info);
	ret_content = ft_print_ptr_content(ptr, nbrlen, info, 1);
	if (info.left)
		ret_pad = ft_apply_padding(content_len, info);
	return (ft_return_check(ret_pad, ret_content));
}

int	ft_print_hex_content(unsigned long nbr, int nbrlen,
	t_flags info, int is_upper)
{
	int	ret_prefix;
	int	ret_precision;
	int	ret_digits;

	ret_prefix = 0;
	if (nbr != 0)
		ret_prefix = ft_apply_prefix(is_upper, 0, info);
	if (ret_prefix == -1)
		return (-1);
	ret_precision = ft_apply_precision(nbrlen, info);
	if (ret_precision == -1)
		return (-1);
	ret_digits = 0;
	if (nbrlen > 0)
	{
		if (is_upper == 0)
			ret_digits = ft_print_nbr_base(nbr, "0123456789abcdef", nbrlen);
		else
			ret_digits = ft_print_nbr_base(nbr, "0123456789ABCDEF", nbrlen);
	}
	return (ft_return_check(ret_digits, ret_prefix + ret_precision));
}

int	ft_print_hex(unsigned int nbr, t_flags info, int is_upper)
{
	int	nbrlen;
	int	precise_len;
	int	ret_pad;
	int	ret_content;

	ret_pad = 0;
	ret_content = 0;
	precise_len = ft_calc_precise_len((unsigned long)nbr, "0123456789ABCDEF",
			info, &nbrlen);
	if (info.prefix && nbr != 0)
		precise_len += 2;
	if (!info.left)
		ret_pad = ft_apply_padding(precise_len, info);
	ret_content = ft_print_hex_content((unsigned long)nbr, nbrlen,
			info, is_upper);
	if (info.left)
		ret_pad = ft_apply_padding(precise_len, info);
	return (ft_return_check(ret_pad, ret_content));
}
