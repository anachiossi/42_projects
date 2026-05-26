/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:25:03 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/25 17:52:41 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_match_needle(const char *haystack, const char *needle,
		size_t len, size_t h)
{
	size_t	n;

	n = 0;
	if (h + ft_strlen(needle) > len)
		return (0);
	while (h < len && needle[n] && haystack[h] == needle[n])
	{
		n++;
		h++;
	}
	if (needle[n] == '\0')
		return (1);
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	h;
	char	*ptr;

	h = 0;
	ptr = NULL;
	if (needle[0] == '\0')
		return ((char *)haystack);
	while (h < len && haystack[h])
	{
		if (haystack[h] == needle[0])
		{
			ptr = (char *)haystack + h;
			if (ft_match_needle(haystack, needle, len, h) == 1)
				return (ptr);
		}
		h++;
	}
	return (NULL);
}
