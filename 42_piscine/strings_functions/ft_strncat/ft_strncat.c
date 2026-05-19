/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:26:47 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 11:14:45 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C03 - ex03
** ft_strncat
** Allowed functions: None
** Key concepts: Strings, Bounded concatenation, Null terminator
**
** Description:
** Reproduce the behavior of strncat (man strncat).
** Appends at most nb bytes from src to dest. The result is always
** null-terminated.
** Input: char *dest (destination), char *src (source), unsigned int nb (max bytes)
** Output: char * (pointer to dest)
** Example: ft_strncat("hello ", "world", 3) → "hello wor"
*/
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
char *ft_strncat(char *dest, char *src, unsigned int nb)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0' && j < nb)
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*
int	main(int argc, char *argv[])
{
	char	dest[100];
	strcpy(dest, argv[1]);
	char	*res;

	if (argc != 4)
	{
		printf("Usage: %s <str1> <str2> [<len>]\n", argv[0]);
		return (1);
	}
	else
	{
		printf("*dest:%s\n", dest);
		res = ft_strncat(dest, argv[2], atoi(argv[3]));
		printf("*src:%s\n", argv[2]);
                printf("after function:%s\n", res);
        }
        return (0);
}*/
