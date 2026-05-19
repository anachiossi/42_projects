#include <stdio.h>

int	ft_recursive_power(int nb, int power);

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
}
