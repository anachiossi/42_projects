/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlowcase.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 09:20:16 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/18 09:54:58 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex08
** ft_strlowcase
** Allowed functions: None
** Key concepts: Strings, Character conversion, ASCII
**
** Description:
** Write a function that converts every uppercase letter in a string to
** lowercase and returns the modified string.
** Input: char *str (the string to convert, modified in place)
** Output: char * (pointer to the modified string)
** Example: ft_strlowcase("HELLO") → "hello"
*/
//#include <stdio.h>
char	*ft_strlowcase(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= 'A') && (str[i] <= 'Z'))
		{
			str[i] = str[i] + 32;
		}
		i++;
	}
	return (str);
}
/*int main(int argc, char **argv)
{
        if (argc != 2)
        {
                printf("Please give me a string!\n");
                return (1);
        }
        printf("original:  %s\n", argv[1]);
        printf("lowercase: %s\n", ft_strlowcase(argv[1]));
        return (0);
}*/
