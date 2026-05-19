/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 16:42:36 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/24 17:33:11 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C04 - ex05
** ft_atoi_base
** Allowed functions: None
** Key concepts: Strings, Base conversion, Sign handling
**
** Description:
** Write a function that converts a string in a given base to an integer.
** Leading whitespace and sign characters (+/-) are skipped, with each '-'
** toggling the sign.
** Input: char *str (the string to convert), char *base (the base string)
** Output: int (the converted integer)
** Rules:
** - Returns 0 if the base is invalid (less than 2 chars, contains '+'/'-',
**   or has duplicate characters)
** Example: ft_atoi_base("FF", "0123456789ABCDEF") → 255
*/

//#include <stdio.h>

int	ft_base_control(char *base)
{
	int	i;
	int	j;
	int	base_len;

	i = 0;
	base_len = 0;
	while (base[i])
	{
		if (base[i] == '+' || base[i] == '-')
			return (0);
		j = i + 1;
		while (base[j])
		{
			if (base[j] == base[i])
				return (0);
			j++;
		}
		i++;
		base_len++;
	}
	return (base_len);
}

int	ft_skip(char *str, int *neg)
{
	int	i;

	i = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\r'
		|| str[i] == '\f' || str[i] == '\v' || str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			*neg = !*neg;
		i++;
	}
	return (i);
}

int	ft_atoi_base(char *str, char *base)
{
	int	base_len;
	int	nbr;
	int	neg;
	int	i;
	int	j;

	nbr = 0;
	neg = 0;
	base_len = ft_base_control(base);
	if (base_len == 0 || base_len == 1)
		return (0);
	i = ft_skip(str, &neg);
	while (str[i])
	{
		j = 0;
		while (base[j] && base[j] != str[i])
			j++;
		if (!base[j])
			break ;
		nbr = nbr * base_len + j;
		i++;
	}
	if (neg == 1)
		nbr = -nbr;
	return (nbr);
}
/*
int	main(void)
{
	char	*str;
	char	*base;
	int	nbr;

	str = "2A";
	base = "0123456789ABCDEF";
	printf("str: %s, base: %s\n", str, base);
	nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "-2A";
		base = "0123456789ABCDEF";
	printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "0";
		base = "0123456789ABCDEF";
	printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "101";
		base = "01";
	printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "-- + $#@#$ - + 42";
		base = "0123456789ABCDEF";
	printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "poneyvif";
		base = "012345678";
	printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "42";
		base = "";
	nbr = printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);

	str = "--42";
		base = "0123456789";
	printf("str: %s, base: %s\n", str, base);
		nbr = ft_atoi_base(str, base);
	printf("atoi_base: %d\n", nbr);
	return (0);
}*/
