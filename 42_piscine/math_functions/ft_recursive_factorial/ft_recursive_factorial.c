/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_recursive_factorial.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/24 18:00:35 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/24 18:01:44 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C05 - ex01
** ft_recursive_factorial
** Allowed functions: None
** Key concepts: Math, Recursion, Base Case
**
** Description:
** Create a recursive function that returns the factorial of nb.
** Input: int nb (the number to calculate factorial for)
** Output: int (the factorial result)
** Rules:
** - If nb is negative or invalid, return 0
** - Overflows are undefined behavior (don't handle them)
** - Base case: 0! = 1
** Example: ft_recursive_factorial(5) = 120
*/
//#include <stdio.h>

int	ft_recursive_factorial(int nb)
{
	int	res;

	if (nb < 0)
		return (0);
	if (nb == 0)
		return (1);
	res = nb * ft_recursive_factorial(nb - 1);
	return (res);
}
/*
int	main(void)
{
	printf("Test 1 - factorial(0):\n");
	printf("Result: %d\n", ft_recursive_factorial(0));
	printf("Expected: 1\n\n");
	printf("Test 2 - factorial(5):\n");
	printf("Result: %d\n", ft_recursive_factorial(5));
	printf("Expected: 120\n\n");
	printf("Test 3 - factorial(10):\n");
	printf("Result: %d\n", ft_recursive_factorial(10));
	printf("Expected: 3628800\n\n");
	printf("Test 4 - factorial(-5):\n");
	printf("Result: %d\n", ft_recursive_factorial(-5));
	printf("Expected: 0\n\n");
	printf("Test 5 - factorial(1):\n");
	printf("Result: %d\n", ft_recursive_factorial(1));
	printf("Expected: 1\n\n");
	printf("Test 6 - factorial(12):\n");
	printf("Result: %d\n", ft_recursive_factorial(12));
	printf("Expected: 479001600\n\n");
	return (0);
}*/
