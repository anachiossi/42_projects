/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_iterative_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 17:46:08 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/24 17:46:56 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C05 - ex00
** ft_iterative_factorial
** Allowed functions: None
** Key concepts: Math, Iteration, Loops
**
** Description:
** Create an iterative function that returns the factorial of nb.
** Input: int nb (the number to calculate factorial for)
** Output: int (the factorial result)
** Rules:
** - If nb is negative or invalid, return 0
** - Overflows are undefined behavior (don't handle them)
** - 0! = 1 by definition
** Example: ft_iterative_factorial(5) = 120
*/
//#include <stdio.h>

int	ft_iterative_factorial(int nb)
{
	int	res;

	res = 1;
	if (nb < 0)
		return (0);
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		res = nb * res;
		nb = nb - 1;
	}
	return (res);
}
/*
int	main(void)
{
	printf("Test 1 - factorial(0):\n");
	printf("Result: %d\n", ft_iterative_factorial(0));
	printf("Expected: 1\n\n");

	printf("Test 2 - factorial(5):\n");
	printf("Result: %d\n", ft_iterative_factorial(5));
	printf("Expected: 120\n\n");

	printf("Test 3 - factorial(10):\n");
	printf("Result: %d\n", ft_iterative_factorial(10));
	printf("Expected: 3628800\n\n");

	printf("Test 4 - factorial(-5):\n");
	printf("Result: %d\n", ft_iterative_factorial(-5));
	printf("Expected: 0\n\n");

	printf("Test 5 - factorial(1):\n");
	printf("Result: %d\n", ft_iterative_factorial(1));
	printf("Expected: 1\n\n");

	printf("Test 6 - factorial(12):\n");
	printf("Result: %d\n", ft_iterative_factorial(12));
	printf("Expected: 479001600\n\n");

	return (0);
}*/
