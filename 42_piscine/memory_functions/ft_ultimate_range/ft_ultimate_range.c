/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_range.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 14:40:22 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/31 10:10:20 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*c07_ex02
 * ft_ultimate_range
 * its a way to not only create the int array from min to max,
 *  but to return the range.
 *   but for a function to output 2 stuff it does like:
 *    in this puntatore ive created your array, but i will return the range itself.
 * if min > max = range will point to NULL and return 0.
 * the size of range should be returned (or -1 on error).
*/

#include <stdlib.h>
#include <unistd.h>

int	ft_ultimate_range(int **range, int min, int max)
{
	int	*result;
	int	size;
	int	i;

	i = 0;
	size = max - min;
	if (min >= max)
	{
		*range = NULL;
		return (0);
	}
	result = (int *)malloc(sizeof(int) * size);
	if (result == NULL)
		return (-1);
	while (min < max)
	{
		result[i] = min;
		i++;
		min++;
	}
	*range = result;
	return (size);
}

/*
void	ft_putnbr(int	nb)
{
	char	digit;

	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		return ;
	}
	if (nb < 0 && nb != -2147483648)
	{
		write(1, "-", 1);
		nb = -nb;
	}
	if (nb >= 10)
		ft_putnbr(nb / 10);
	digit = nb % 10 + '0';
	write(1, &digit, 1);
	return ;
}

int	main(void)
{
	int	*arr;
	int	size;
	int	i;

	i = 0;
	size = ft_ultimate_range(&arr, -42, 42);
	ft_putnbr(size);
	write(1, "\n", 1);
	while (i < size)
	{
		ft_putnbr(arr[i]);
		write(1, ", ", 2);
		i++;
	}
	write(1, "\n", 1);
	free(arr);
	return (0);
}*/
