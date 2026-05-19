/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strupcase.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 09:18:21 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/18 09:42:11 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex07
** ft_strupcase
** Allowed functions: None
** Key concepts: Strings, Character conversion, ASCII
**
** Description:
** Write a function that converts every lowercase letter in a string to
** uppercase and returns the modified string.
** Input: char *str (the string to convert, modified in place)
** Output: char * (pointer to the modified string)
** Example: ft_strupcase("hello") → "HELLO"
*/
//#include <stdio.h>
char	*ft_strupcase(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= 'a') && (str[i] <= 'z'))
		{
			str[i] = str[i] - 32;
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
        printf("uppercase: %s\n", ft_strupcase(argv[1]));
        return (0);
}*/
