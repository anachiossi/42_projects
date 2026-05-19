/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:40:32 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 16:06:32 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C01 - ex02
** ft_swap
** Allowed functions: None
** Key concepts: Pointers, Value swapping
**
** Description:
** Write a function that swaps the values of two integers using their addresses.
** Input: int *a, int *b (pointers to the integers to swap)
** Output: void
** Example: ft_swap(&a, &b) → a and b have exchanged values
*/
/*#include <stdio.h>*/

void	ft_swap(int *a, int *b)
{
	int	temporary;

	temporary = *a;
	*a = *b;
	*b = temporary;
}
/*
int	main(void)
{
	int	a;
	int	b;

	a = 69;
	b = 42;
	printf("before: %d, %d\n", a, b);
	ft_swap(&a, &b);
	printf("after swap: %d, %d\n", a, b);
	return (0);
}*/
