/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:37:04 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/26 16:24:02 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C06 - ex03
** ft_sort_params
** Allowed functions: write
** Key concepts: argc/argv, bubble sort, string comparison,
**               sorting algorithm with nested loops
** Description: Display all command-line arguments except argv[0],
**              sorted in ASCII order, each on a new line.
** Example: ./a.out zebra apple banana → outputs: apple\nbanana\nzebra\n
*/

#include <unistd.h>

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while((s1[i] == s2[i]) && s1[i] !='\0')
	       i++;
	return (s1[i] - s2[i]);
}	

void	print_argv(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while(argv[i])
	{
		j = 0;
		while(argv[i][j])
		{
			write(1, &argv[i][j], 1);
			j++;
		}
		write(1, "\n", 1);
		i++;
	}
}

int	main(int argc, char **argv)
{
	int	i;
	int	j;
	char	*temp;

	j = argc - 1;
	if (argc < 2)
		return (0);
	while (j > 0)
	{
		i = 1;
		while (i < (argc - 1))
		{
			if (ft_strcmp(argv[i], argv[i + 1]) > 0)
			{
				temp = argv[i];
				argv[i] = argv[i + 1];
				argv[i + 1] = temp;
			}
			i++;
		}
		j--;
	}
	print_argv(argv);
	return (0);
}
