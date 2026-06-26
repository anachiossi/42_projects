/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:24:31 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 12:14:23 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*ptr;
	size_t				i;

	i = 0;
	ptr = s;
	while (i < n)
	{
		if (ptr[i] == (unsigned char)c)
			return ((void *)(ptr + i));
		i++;
	}
	return (NULL);
}
