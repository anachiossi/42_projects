/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:28:00 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 17:09:38 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_itoa_size(int n)
{
	long	nb;
	size_t	size;

	nb = (long)n;
	size = 0;
	if (nb < 0)
	{
		size++;
		nb = -nb;
	}
	if (nb == 0)
		size++;
	while (nb > 0)
	{
		nb = nb / 10;
		size++;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	char	*nb_str;
	size_t	size;
	long	nb;

	nb = (long)n;
	size = ft_itoa_size(n);
	nb_str = (char *)malloc(sizeof(char) * (size + 1));
	if (nb_str == NULL)
		return (NULL);
	if (nb == 0)
		nb_str[0] = '0';
	nb_str[size] = '\0';
	if (nb < 0)
	{
		nb = -nb;
		nb_str[0] = '-';
	}
	while (nb > 0)
	{
		nb_str[size - 1] = (nb % 10) + '0';
		nb = nb / 10;
		size--;
	}
	return (nb_str);
}
