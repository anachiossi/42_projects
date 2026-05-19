/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 09:05:30 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/18 13:44:03 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex01
** ft_strncpy
** Allowed functions: None
** Key concepts: Strings, Bounded copy, Null padding
**
** Description:
** Reproduce the behavior of strncpy (man strncpy).
** Copies at most n bytes from src into dest. If src is shorter than n bytes,
** the remainder of dest is filled with null bytes.
** Input: char *dest (destination), char *src (source), unsigned int n (max bytes)
** Output: char * (pointer to dest)
** Example: ft_strncpy(dest, "hello", 3) → dest contains "hel"
*/
//#include <stdio.h>
//#include <stdlib.h>

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while ((i < n) && (src[i] != '\0'))
	{
		dest[i] = src[i];
		i++;
	}
	while (i != n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}
/*int	main(int argc, char **argv)
{
	char	dest[100];
	unsigned int	n;

	if (argc != 3)
	{
		printf("Please give me a string and an int.\n");
		return (1);
	}
	n = atoi(argv[2]);
	ft_strncpy(dest, argv[1], n);
	printf("src: %s\n", argv[1]);
	printf("unsigned int: %d\n", n);
	printf("dest: %s\n", dest);
	return (0);
}*/
