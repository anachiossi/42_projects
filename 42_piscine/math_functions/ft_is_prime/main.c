#include <stdio.h>

int	ft_is_prime(int nb);

int	main(void)
{
	printf("is_prime(0)  = %d (expected 0)\n", ft_is_prime(0));
	printf("is_prime(1)  = %d (expected 0)\n", ft_is_prime(1));
	printf("is_prime(2)  = %d (expected 1)\n", ft_is_prime(2));
	printf("is_prime(7)  = %d (expected 1)\n", ft_is_prime(7));
	printf("is_prime(4)  = %d (expected 0)\n", ft_is_prime(4));
	printf("is_prime(97) = %d (expected 1)\n", ft_is_prime(97));
	printf("is_prime(-5) = %d (expected 0)\n", ft_is_prime(-5));
	return (0);
}
