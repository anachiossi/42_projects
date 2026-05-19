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
/*
** C04 - ex00
** ft_strlen
** Allowed functions: None
** Key concepts: Strings, Loops, Null terminator
**
** Description:
** Write a function that counts and returns the number of characters in a
** string, not including the null terminator.
** Input: char *str (the string to measure)
** Output: int (the length of the string)
** Example: ft_strlen("Hello") → 5
*/
int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}
