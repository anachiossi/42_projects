/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 18:49:39 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 11:55:52 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C03 - ex01
** ft_strncmp
** Allowed functions: None
** Key concepts: Strings, Bounded comparison
**
** Description:
** Reproduce the behavior of strncmp (man strncmp).
** Compares at most the first n bytes of s1 and s2.
** Input: char *s1, char *s2 (strings to compare), unsigned int n (max bytes)
** Output: int (0 if equal up to n, negative if s1 < s2, positive if s1 > s2)
** Example: ft_strncmp("abcd", "abce", 3) → 0
*/
//#include <stdio.h>
//#include <stdlib.h>
int	ft_strncmp(char *s1, char *s2, unsigned int n)
{
	int	i;

	i = 0;
	if (n == 0)
		return (0);
	while (s1[i] == s2[i] && s1[i] !='\0' && i < n)
	{
		i++;
	}
	if (i == n)
		return (0);
	else
		return(s1[i] - s2[i]);
}
/*
int     main(int argc, char *argv[])
{
        int res;

        if (argc != 4 )
        {
	printf("Usage: %s <str1> <str2> [<len>]\n", argv[0]);
                return (1);
        }
        else
                res = ft_strncmp(argv[1], argv[2], atoi(argv[3]));
        if (res == 0)
                printf("<s1> and <s2> are equal\n");
        else if (res < 0)
                printf("<s1> is less than <s2> by (%d)\n", res);
        else
                printf("<s1> is greater than <s2> by (%d)\n", res);
        return (0);
}*/
