#include <stdio.h>

int	ft_fibonacci(int index);

int	main(void)
{
	printf("fibonacci(0)  = %d (expected 0)\n", ft_fibonacci(0));
	printf("fibonacci(1)  = %d (expected 1)\n", ft_fibonacci(1));
	printf("fibonacci(2)  = %d (expected 1)\n", ft_fibonacci(2));
	printf("fibonacci(4)  = %d (expected 3)\n", ft_fibonacci(4));
	printf("fibonacci(10) = %d (expected 55)\n", ft_fibonacci(10));
	printf("fibonacci(-1) = %d (expected -1)\n", ft_fibonacci(-1));
	return (0);
}
