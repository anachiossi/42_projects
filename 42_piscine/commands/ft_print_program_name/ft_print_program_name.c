/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_program_name.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/25 16:31:48 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/26 11:16:30 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C06 - ex00
** ft_print_program_name
** Allowed functions: write
** Key concepts: argc/argv, program name access, write() syscall
** Description: Display the program's own name (argv[0]) followed by a newline.
** Example: ./a.out → outputs: ./a.out\n
** Verified examples: 
**   - Input: ./a.out | Output: ./a.out$ (with newline)
*/

#include <unistd.h>

int	main(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc < 1)
		return (0);
	while (argv[0][i])
	{
		write(1, &argv[0][i], 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
