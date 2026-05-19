/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_range.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 10:18:12 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/30 12:34:13 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C07 - ex01 - ft_range
** Allowed: malloc
**
** Creates an int array filled from min to max (max excluded).
** Returns NULL if min >= max.
**
** ft_range(1, 5)  → [1, 2, 3, 4]
** ft_range(5, 1)  → NULL
**
** malloc((max - min) * sizeof(int))
**
** int *ft_range(int min, int max)
*/

#include <stdlib.h>
#include <unistd.h>
//#include <stdio.h>

int	*ft_range(int min, int max)
{
	int	*result;
	int	range;
	int	i;

	i = 0;
	if (min >= max)
                return (NULL);
	range = max - min;
	result = (int *)malloc(sizeof(int) * range);
	if (result == NULL)
		return (NULL);
	while (min < max)
	{
		result[i] = min;
		i++;
		min++;
	}
	return (result);
}

/*int	ft_atoi(char *str)
{
	int	i;
	int	nb;
	int	negative;

	i = 0;
	nb = 0;
	negative = 0;
        while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
                        || str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
                i++;
        while (str[i] == '-' || str[i] == '+')
        {
                if (str[i] == '-')
                        negative = !negative;
                i++;
        }
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	if (negative)
		nb = -nb;
	return (nb);
}

int	main(int argc, char **argv)
{
	int	min;
	int	max;
	int	*result;
	int	range;
	int	i;

	i = 0;
	if (argc < 3)
		return (0);
	min = ft_atoi(argv[1]);
	max = ft_atoi(argv[2]);
	range = max - min;
	result = ft_range(min, max);
	if (result == NULL)
	{
		printf("NULL\n");
		return (0);
	}
	while (i < range)
	{
		printf("%d ", result[i]);
		i++;
	}
	printf("\n");
	free(result);
	return (0);
}*/
