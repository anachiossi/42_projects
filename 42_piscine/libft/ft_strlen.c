/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:45:20 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 14:12:16 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*c04 - ex00
 * Crea una funzione che conta e restituisce
 * il numero di caratteri in una stringa.
 * 	none*/
int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}
