/*
** C05 - ex04
** ft_fibonacci
** Allowed functions: None
** Key concepts: Recursion, Sequences, Math
**
** Description:
** Create a recursive function that returns the n-th element of the Fibonacci sequence.
** Sequence: 0, 1, 1, 2, 3, 5, 8, 13...
** Input: int index (position in sequence, starts at 0)
** Output: int (the Fibonacci number at that index)
** Rules:
** - If index < 0, return -1
** - Overflows are undefined behavior (don't handle them)
** - Index 0 = 0, Index 1 = 1, Index 2 = 1, Index 3 = 2, etc.
** Example: ft_fibonacci(4) = 3
*/

//#include <stdio.h>

int	ft_fibonacci(int index)
{
	int	prev;
	int	prev_prev;

	if (index < 0)
		return (-1);
	if (index == 0)
		return (0);
	if (index == 1)
		return (1);
	prev = ft_fibonacci(index - 1);
	prev_prev = ft_fibonacci(index - 2);
	return (prev + prev_prev);
}

/*
int	main(void)
{
	printf("fibonacci(0)  = %d (expected 0)\n", ft_fibonacci(0));
	printf("fibonacci(1)  = %d (expected 1)\n", ft_fibonacci(1));
	printf("fibonacci(2)  = %d (expected 1)\n", ft_fibonacci(2));
	printf("fibonacci(4)  = %d (expected 3)\n", ft_fibonacci(4));
	printf("fibonacci(10) = %d (expected 55)\n", ft_fibonacci(10));
	printf("fibonacci(-1) = %d (expected -1)\n", ft_fibonacci(-1));
	return (0);
}*/
