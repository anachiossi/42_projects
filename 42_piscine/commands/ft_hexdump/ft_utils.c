/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 16:22:04 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/18 14:32:50 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_hexdump.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_print_error(char *program, char *filename)
{
	char	*file;
	char	*error;
	char	*exec;

	exec = basename(program);
	file = basename(filename);
	error = strerror(errno);
	write(2, exec, ft_strlen(exec));
	write(2, ": ", 2);
	write(2, file, ft_strlen(file));
	write(2, ": ", 2);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	return ;
}

void	ft_memcpy(unsigned char *previous, unsigned char *buffer, int bytes)
{
	int	i;

	i = 0;
	while (i < bytes)
	{
		previous[i] = buffer[i];
		i++;
	}
}

int	ft_memcmp(unsigned char *previous, unsigned char *buffer, int bytes)
{
	int	i;

	i = 0;
	while (i < bytes)
	{
		if (previous[i] != buffer[i])
			return (1);
		i++;
	}
	return (0);
}
