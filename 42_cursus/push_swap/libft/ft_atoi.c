/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:50:03 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 12:11:20 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f'
		|| c == '\v')
		return (1);
	return (0);
}

static int	ft_issign(int c)
{
	if (c == '+' || c == '-')
		return (1);
	return (0);
}

int	ft_atoi(const char *nptr)
{
	int		i;
	int		negative;
	long	result;

	i = 0;
	result = 0;
	negative = 0;
	while (ft_isspace(nptr[i]))
		i++;
	if (ft_issign(nptr[i]))
	{
		if (nptr[i] == '-')
			negative = 1;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	if (negative)
		return ((int)(-result));
	return ((int)result);
}
