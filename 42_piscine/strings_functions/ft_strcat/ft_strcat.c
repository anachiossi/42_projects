/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:26:47 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 11:14:02 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C03 - ex02
** ft_strcat
** Allowed functions: None
** Key concepts: Strings, Concatenation, Null terminator
**
** Description:
** Reproduce the behavior of strcat (man strcat).
** Appends src to dest, overwriting the null terminator at the end of dest.
** Dest must have enough space for the result.
** Input: char *dest (destination, must have enough space), char *src (source)
** Output: char * (pointer to dest)
** Example: ft_strcat("hello ", "world") → "hello world"
*/
//#include <stdio.h>
//#include <stdlib.h>
char *ft_strcat(char *dest, char *src)
{
	unsigned int	i;
	unsigned int	j;

	i = 0;
	j = 0;
	while (dest[i] != '\0')
		i++;
	while (src[j] != '\0')
	{
		dest[i] = src[j];
		j++;
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
/*int     main(int argc, char *argv[])
{
	char *res;

        if (argc < 3)
        {
                printf("Usage: %s <str1> <str2>\n", argv[0]);
                return (1);
        }
	else
	{
		printf("*dest:%s\n", argv[1]);
		printf("*src:%s\n", argv[2]);
		res = ft_strcat(argv[1], argv[2]);
		printf("after function:%s\n", res);
	}
        return (0);
}*/
