/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_is_uppercase.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 09:14:46 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/17 16:38:49 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex05
** ft_str_is_uppercase
** Allowed functions: None
** Key concepts: Strings, Character classification
**
** Description:
** Write a function that returns 1 if the string contains only uppercase
** alphabetical characters, and 0 if it contains any other character.
** Input: char *str (the string to check)
** Output: int (1 if all uppercase, 0 otherwise)
** Rules:
** - Returns 1 if str is empty
** Example: ft_str_is_uppercase("HELLO") → 1, ft_str_is_uppercase("Hello") → 0
*/
//#include <stdio.h>

int	ft_str_is_uppercase(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= 'A') && (str[i] <= 'Z'))
			i++;
		else
			return (0);
	}
	return (1);
}
/*int     main(int argc, char **argv)
{
        if (argc != 2)
        {
                printf("Please give me a string.\n");
                return (1);
        }
        if (ft_str_is_uppercase(argv[1]))
                printf("1\n");
        else
                printf("0\n");
        return (0);
}*/
