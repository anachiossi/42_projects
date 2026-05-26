/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 12:13:11 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 12:13:23 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*ptr;
	size_t			i;
	size_t			array_size;

	i = 0;
	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (NULL);
	array_size = nmemb * size;
	if (array_size == 0)
	{
		ptr = malloc(1);
		if (ptr == NULL)
			return (NULL);
		ptr[0] = 0;
		return ((void *)ptr);
	}
	ptr = malloc(array_size);
	if (ptr == NULL)
		return (NULL);
	while (i < array_size)
	{
		ptr[i] = 0;
		i++;
	}
	return ((void *)ptr);
}
