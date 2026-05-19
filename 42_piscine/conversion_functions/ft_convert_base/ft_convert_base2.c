/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert_base2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 14:58:55 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/31 17:42:16 by anade-mo         ###   ########.fr       */
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
#include <stdio.h>

int	ft_base_control_len(char *base);
int	ft_atoi_base(char *str, char *base);


int	ft_nbrlen(long nbr, int base_len)
{
	int	nbrlen;

	nbrlen = 0;
	if (nbr <= 0)
		nbr = -nbr;
	if (nbr == 0)
		return (0);
	while (nbr > 0)
	{
		nbr = nbr / base_len;
		nbrlen++;
	}
	return (nbrlen);
}

char	*ft_itoa_base(long nbr, char *base_to, char *output)
{
	int	i;
	int	baselen;
	int	nbrlen;
	
	baselen = ft_base_control_len(base_to);
	nbrlen = ft_nbrlen(nbr, baselen);
	if (nbr < 0)
	{
		nbr = -nbr;
		output[0] = '-';
		i = nbrlen;
        }
	else
		i = nbrlen - 1;
	while (nbr > 0)
	{
		output[i] = base_to[nbr % baselen];
		nbr = nbr / baselen;
		i--;
	}
	return (output);
}

char	*ft_convert_base(char *nbr, char *base_from, char *base_to)
{
	long	value;
	char	*output;
	int	nbrlen;

	if (ft_base_control_len(base_from) == 0 
			|| ft_base_control_len(base_to) == 0)
		return (NULL);
	value = ft_atoi_base(nbr, base_from);
	nbrlen = ft_nbrlen(value, ft_base_control_len(base_to));
	output = (char*)malloc(sizeof(char) * nbrlen + 2);
	if (output == NULL)
		return NULL;
	output = ft_itoa_base(value, base_to, output);
	if (value < 0)
		output[nbrlen + 1] = '\0';
	else
		output[nbrlen] = '\0';
	return (output);
}

int	main(int argc, char **argv)
{
	char	*base_from;
	char	*base_to;
	char	*nbr;
	char	*result;

	if (argc < 4)
	{
		write(1, "needs 4 argv", 12);
		return (1);
	}
	nbr = argv[1];
	base_from = argv[2];
	base_to = argv[3];
	result = ft_convert_base(nbr, base_from, base_to);
	if (result)
		printf("%s\n", result);
	else
		printf("NULL\n");
	return (0);
}

