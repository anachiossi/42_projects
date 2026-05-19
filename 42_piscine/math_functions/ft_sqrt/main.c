#include <stdio.h>

int	ft_sqrt(int nb);

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
}
