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
/* c03
 * ex00
 * Riproduci il comportamento della funzione strcmp (man strcmp)
 * The  strcmp()  function compares the two strings s1 and s2.  The locale
		is not taken into account (for  a  locale-aware  comparison,  see  str‐
		coll(3)).  The comparison is done using unsigned characters.

		strcmp() returns an integer indicating the result of the comparison, as
		follows:

		• 0, if the s1 and s2 are equal;

		• a negative value if s1 is less than s2;

		• a positive value if s1 is greater than s2.

		The strncmp() function is similar,
		except it compares  only  the  first
		(at most) n bytes of s1 and s2.
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
