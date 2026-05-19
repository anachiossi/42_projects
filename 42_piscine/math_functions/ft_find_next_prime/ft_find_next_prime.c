/*
** C05 - ex07
** ft_find_next_prime
** Allowed functions: None
** Key concepts: Math, Prime Numbers, Search
**
** Description:
** Create a function that returns the next prime number >= nb.
** Input: int nb (starting point)
** Output: int (the next prime number >= nb)
** Rules:
** - Return the first prime >= nb (including nb itself if it's prime)
** Example: ft_find_next_prime(10) = 11, ft_find_next_prime(11) = 11
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

int	ft_find_next_prime(int nb)
{
	int	i;

	i = 0;
	while (!ft_is_prime(nb + i))
		i++;
	return (nb + i);
}

/*
int	main(void)
{
	printf("next_prime(0)   = %d (expected 2)\n", ft_find_next_prime(0));
	printf("next_prime(2)   = %d (expected 2)\n", ft_find_next_prime(2));
	printf("next_prime(10)  = %d (expected 11)\n", ft_find_next_prime(10));
	printf("next_prime(11)  = %d (expected 11)\n", ft_find_next_prime(11));
	printf("next_prime(13)  = %d (expected 13)\n", ft_find_next_prime(13));
	printf("next_prime(100) = %d (expected 101)\n", ft_find_next_prime(100));
	return (0);
}*/
