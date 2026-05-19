/*
** C05 - ex03
** ft_recursive_power
** Allowed functions: None
** Key concepts: Math, Recursion, Exponentiation
**
** Description:
** Create a recursive function that returns nb raised to the power of 'power'.
** Input: int nb (base), int power (exponent)
** Output: int (result of nb^power)
** Rules:
** - If power < 0, return 0
** - By definition, 0^0 = 1
** - Overflows are undefined behavior (don't handle them)
** Example: ft_recursive_power(2, 3) = 8
*/

//#include <stdio.h>

int	ft_recursive_power(int nb, int power)
{
	int	res;

	if (power < 0)
		return (0);
	if (power == 0)
		return (1);
	res = nb * ft_recursive_power(nb, power - 1);
	return (res);
}

/*
int	main(void)
{
	printf("power(2, 0)  = %d (expected 1)\n", ft_recursive_power(2, 0));
	printf("power(2, 3)  = %d (expected 8)\n", ft_recursive_power(2, 3));
	printf("power(5, 2)  = %d (expected 25)\n", ft_recursive_power(5, 2));
	printf("power(10, 3) = %d (expected 1000)\n", ft_recursive_power(10, 3));
	printf("power(2, 10) = %d (expected 1024)\n", ft_recursive_power(2, 10));
	printf("power(-2, 3) = %d (expected -8)\n", ft_recursive_power(-2, 3));
	printf("power(-2, 2) = %d (expected 4)\n", ft_recursive_power(-2, 2));
	printf("power(0, 0)  = %d (expected 1)\n", ft_recursive_power(0, 0));
	printf("power(5, -1) = %d (expected 0)\n", ft_recursive_power(5, -1));
	printf("power(1, 100)= %d (expected 1)\n", ft_recursive_power(1, 100));
	return (0);
}*/
