/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_is_printable.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 09:16:48 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/17 17:00:49 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C02 - ex06
** ft_str_is_printable
** Allowed functions: None
** Key concepts: Strings, Character classification, ASCII
**
** Description:
** Write a function that returns 1 if the string contains only printable
** characters (ASCII 32 to 126), and 0 if it contains any other character.
** Input: char *str (the string to check)
** Output: int (1 if all printable, 0 otherwise)
** Rules:
** - Returns 1 if str is empty
** Example: ft_str_is_printable("hello!") → 1, ft_str_is_printable("hi\n") → 0
*/
int	ft_str_is_printable(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if ((str[i] >= ' ') && (str[i] <= '~'))
			i++;
		else
			return (0);
	}
	return (1);
}
/*int     ft_str_is_printable(char *str);

int     main(void)
{
        printf("%d\n", ft_str_is_printable("Ana won $100.00!"));
        printf("%d\n", ft_str_is_printable("Ana :)"));
        printf("%d\n", ft_str_is_printable("ana@gmail.com"));
        printf("%d\n", ft_str_is_printable(""));
        return (0);
}*/
