/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 08:53:54 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/17 12:11:01 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex00
** ft_strcpy
** Allowed functions: None
** Key concepts: Strings, Copying, Null terminator
**
** Description:
** Reproduce the behavior of strcpy (man strcpy).
** Copies the string pointed to by src, including the null terminator,
** into the buffer pointed to by dest.
** Input: char *dest (destination buffer), char *src (source string)
** Output: char * (pointer to dest)
** Example: ft_strcpy(dest, "hello") → dest contains "hello"
*/

//#include <stdio.h>

char	*ft_strcpy(char *dest, char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/*int	main(int argc, char **argv)
{
	char	dest[100];

	if (argc != 2)
	{
		printf("Please give me a string!\n");
		return (1);
	}
	ft_strcpy(dest, argv[1]);
	printf("src: %s\n", argv[1]);
	printf("dest: %s\n", dest);
	return (0);
}*/
