/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:28:23 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 18:46:18 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	size_t	i;

	i = 0;
	if (s == NULL)
		return ;
	if (f == NULL)
		return ;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
	return ;
}
