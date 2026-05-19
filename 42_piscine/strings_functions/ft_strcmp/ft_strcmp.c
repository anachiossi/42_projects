/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 14:51:17 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 11:10:52 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
** C03 - ex00
** ft_strcmp
** Allowed functions: None
** Key concepts: Strings, Comparison, Unsigned characters
**
** Description:
** Reproduce the behavior of strcmp (man strcmp).
** Compares strings s1 and s2 using unsigned characters.
** Input: char *s1, char *s2 (the strings to compare)
** Output: int (0 if equal, negative if s1 < s2, positive if s1 > s2)
** Example: ft_strcmp("abc", "abc") → 0, ft_strcmp("a", "b") → negative
*/

//#include <stdio.h>
int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;
	
	i = 0;
	while ((s1[i] == s2[i] && s1[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}
/*int	main(int argc, char *argv[])
{
	int res;

	if (argc < 3)
	{
		printf("Usage: %s <str1> <str2>\n", argv[0]);
		return (1);
	}
	else
		res = ft_strcmp(argv[1], argv[2]);
	if (res == 0)
		printf("<s1> and <s2> are equal\n");
	else if (res < 0)
		printf("<s1> is less than <s2> by (%d)\n", res);
	else
		printf("<s1> is greater than <s2> by (%d)\n", res);
	return (0);
}*/
