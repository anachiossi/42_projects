#include <stdio.h>

int	ft_find_next_prime(int nb);

int	main(void)
{
	printf("next_prime(0)   = %d (expected 2)\n", ft_find_next_prime(0));
	printf("next_prime(2)   = %d (expected 2)\n", ft_find_next_prime(2));
	printf("next_prime(10)  = %d (expected 11)\n", ft_find_next_prime(10));
	printf("next_prime(11)  = %d (expected 11)\n", ft_find_next_prime(11));
	printf("next_prime(13)  = %d (expected 13)\n", ft_find_next_prime(13));
	printf("next_prime(100) = %d (expected 101)\n", ft_find_next_prime(100));
	return (0);
}
