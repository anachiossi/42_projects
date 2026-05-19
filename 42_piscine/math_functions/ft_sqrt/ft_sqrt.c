/*
** C05 - ex05
** ft_sqrt
** Allowed functions: None
** Key concepts: Math, Search, Square Roots
**
** Description:
** Create a function that returns the integer square root of nb.
** Return 0 if the square root is irrational (not a perfect square).
** Input: int nb (the number to find square root for)
** Output: int (the integer square root, or 0 if irrational)
** Rules:
** - Only return a result if nb is a perfect square
** - For irrational square roots, return 0
** Example: ft_sqrt(16) = 4, ft_sqrt(17) = 0
*/

//#include <stdio.h>

int	ft_sqrt(int nb)
{
	int	res;

	res = 0;
	while (res <= nb)
	{
		if (nb == res * res)
			return (res);
		if (nb < res * res)
			return (0);
		res++;
	}
	return (0);
}

/*
int	main(void)
{
	printf("sqrt(0)   = %d (expected 0)\n", ft_sqrt(0));
	printf("sqrt(1)   = %d (expected 1)\n", ft_sqrt(1));
	printf("sqrt(4)   = %d (expected 2)\n", ft_sqrt(4));
	printf("sqrt(16)  = %d (expected 4)\n", ft_sqrt(16));
	printf("sqrt(17)  = %d (expected 0)\n", ft_sqrt(17));
	printf("sqrt(100) = %d (expected 10)\n", ft_sqrt(100));
	printf("sqrt(-5)  = %d (expected 0)\n", ft_sqrt(-5));
	return (0);
}*/
