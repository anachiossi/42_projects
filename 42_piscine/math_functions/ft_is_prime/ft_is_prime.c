/*
** C05 - ex06
** ft_is_prime
** Allowed functions: None
** Key concepts: Math, Prime Numbers, Logic
**
** Description:
** Create a function that returns 1 if nb is a prime number, 0 if not.
** Input: int nb (the number to check)
** Output: int (1 if prime, 0 if not)
** Rules:
** - 0 and 1 are NOT prime numbers
** - A prime number is only divisible by 1 and itself
** Example: ft_is_prime(7) = 1, ft_is_prime(4) = 0
*/

//#include <stdio.h>

int	ft_is_prime(int nb)
{
	int	i;

	i = 2;
	if (nb == 1 || nb == 0 || nb < 0)
		return (0);
	while (i < nb)
	{
		if (nb % i == 0)
			return (0);
		i++;
	}
	return (1);
}

/*
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
}*/
