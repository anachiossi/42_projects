/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 11:26:13 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/31 17:43:04 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*c07 - ex04
 * returns the result of convertting the string nbr from base_from to base_to;
 * nbr, base_from and base_to mus be read only;
 * nbr will follow the rules of atoi_base; (+ -, spaces);
 * nbr must fit and int. 
 * if base is invalid return NULL;
 * invalid bases have = repeated chars, - or + and spaces;
 * if it has less then 2 chars;
 * number should have - if negative, never + or space;
 * good luck;
 *
 */

#include <stdlib.h>
#include <unistd.h>

int	ft_base_control_len(char *base)
{
	int	i;
	int	len;
	int	j;

	len = 0;
	i = 0;
	while (base[len])
		len++;
	if (len <= 1)
		return (0);
       	while (i < len)
	{
		j = i + 1;
		if (base[i] == '+' || base [i] == '-'
				|| base[i] >= '\t' && base[i] <= '\r'
                                || base[i] == ' ')
                        return (0);
		while (j < len)
		{
			if (base[i] == base[j])
				return (0);
                        j++;
		}
		i++;
	}
	return(len);
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
	long	nbr;
	int	len;
	int	i;
	int	index;
	int	negative;

	negative = 0;
	len = ft_base_control_len(base);
	if (len == 0)
		return (0);
	i = ft_skip(str, &negative);
	nbr = 0;
	while (str[i])
	{
		index = 0;
		while (base[index] && base[index] != str[i])
			index++;
		if(!base[index])
			break ;
		nbr = nbr * len + index;
		i++;
	}
	if (negative == 1)
		nbr = -nbr;
	return (nbr);
}

