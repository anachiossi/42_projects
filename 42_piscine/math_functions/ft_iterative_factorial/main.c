#include <stdio.h>

int	ft_iterative_factorial(int nb);

int	main(void)
{
	printf("factorial(0)  = %d (expected 1)\n", ft_iterative_factorial(0));
	printf("factorial(1)  = %d (expected 1)\n", ft_iterative_factorial(1));
	printf("factorial(5)  = %d (expected 120)\n", ft_iterative_factorial(5));
	printf("factorial(10) = %d (expected 3628800)\n", ft_iterative_factorial(10));
	printf("factorial(12) = %d (expected 479001600)\n", ft_iterative_factorial(12));
	printf("factorial(-1) = %d (expected 0)\n", ft_iterative_factorial(-1));
	printf("factorial(-5) = %d (expected 0)\n", ft_iterative_factorial(-5));
	return (0);
}
