/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_is_numeric.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 09:11:09 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/17 17:07:48 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex03
** ft_str_is_numeric
** Allowed functions: None
** Key concepts: Strings, Character classification
**
** Description:
** Write a function that returns 1 if the string contains only digit
** characters, and 0 if it contains any other character.
** Input: char *str (the string to check)
** Output: int (1 if all digits, 0 otherwise)
** Rules:
** - Returns 1 if str is empty
** Example: ft_str_is_numeric("123") → 1, ft_str_is_numeric("12a") → 0
*/
//#include <stdio.h>
int	ft_str_is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= '0') && (str[i] <= '9'))
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
