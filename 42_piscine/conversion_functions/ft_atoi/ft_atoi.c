/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/23 14:25:15 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/24 16:41:20 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C04 - ex03
** ft_atoi
** Allowed functions: None
** Key concepts: Strings, Conversion, Sign handling, Whitespace skipping
**
** Description:
** Write a function that converts a string to an integer, reproducing the
** behavior of atoi (man atoi).
** Leading whitespace is skipped. Multiple signs are handled: each '-' toggles
** the sign, '+' has no effect.
** Input: char *str (the string to convert)
** Output: int (the converted integer)
** Example: ft_atoi("  -42") → -42, ft_atoi("+-42") → -42
*/
//#include <stdio.h>
int	ft_atoi(char *str)
{
	int	i;
	int	negative;
	int	result;

	i = 0;
	negative = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r'
		|| str[i] == '\f' || str[i] == '\v')
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			negative = !negative;
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && (str[i]))
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (negative)
		return (-result);
	return (result);
}
/*
int	main(int argc, char **argv)
{
	int	res;

	if (argc != 2)
		return (1);
	res = (ft_atoi(argv[1]));
	printf("%d\n", res);
	return (0);
}*/
