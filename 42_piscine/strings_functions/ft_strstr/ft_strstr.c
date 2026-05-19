/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:34:46 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 11:15:18 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C03 - ex04
** ft_strstr
** Allowed functions: None
** Key concepts: Strings, Substring search
**
** Description:
** Reproduce the behavior of strstr (man strstr).
** Finds the first occurrence of the substring to_find in str.
** Input: char *str (the string to search in), char *to_find (the substring)
** Output: char * (pointer to the start of the match, or NULL if not found)
** Example: ft_strstr("hello world", "world") → pointer to "world"
*/
//#include <stdio.h>
//#include <stdlib.h>
char	*ft_strstr(char	*str, char *to_find)
{
        unsigned int    i;
        unsigned int    j;
	unsigned int	pointer;

        i = 0;
        j = 0;
        while (str[i] != '\0')
        {
		pointer = i;
		while (str[i] == to_find[j] && to_find[j] != '\0')
		{
			i++;
			j++;
		}
		if (to_find[j] == '\0')
			return (&str[pointer]);
		i = pointer + 1;
	}
return (NULL);
}
/*
int     main(int argc, char *argv[])
{
        char *res;

        if (argc < 3)
        {
                printf("Usage: %s <str1> <str2>\n", argv[0]);
                return (1);
        }
        else
        {
                printf("*src:%s\n", argv[1]);
               	printf("to_find:%s\n", argv[2]);
		res = ft_strstr(argv[1], argv[2]);
                printf("after function:%s\n", res);
        }
        return (0);
}*/
