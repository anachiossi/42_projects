/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 22:19:42 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/13 14:53:20 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C10 - ex01
** ft_cat
** Allowed functions: open, read, write, close, strerror, errno, basename
** Key concepts: File I/O, stdin, Error handling, Multiple files
**
** Description:
** Write a program that reproduces the behavior of the cat command.
** Reads from stdin if no arguments are given, otherwise displays each file.
** Errors for each file are written to stderr using strerror.
** Input: argv[1..n] (optional filenames)
** Output: file contents to stdout, per-file errors to stderr
** Example: ./ft_cat file.txt → displays file.txt content
*/

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <string.h>
#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_display_input(void)
{
	int		fd;
	int		bytes;
	char	buffer[4096];

	fd = 0;
	bytes = read(fd, buffer, 4096);
	while (bytes > 0)
	{
		write(1, buffer, bytes);
		bytes = read(fd, buffer, 4096);
	}
	return ;
}

void	ft_print_error(char *filename)
{
	char	*file;
	char	*error;

	file = basename(filename);
	error = strerror(errno);
	write(2, "ft_cat: ", ft_strlen("ft_cat: "));
	write(2, file, ft_strlen(file));
	write(2, ": ", 2);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	return ;
}

int	ft_display_file(char *filename)
{
	int		fd;
	int		bytes;
	char	buffer[4096];

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_print_error(filename);
		return (-1);
	}
	bytes = read(fd, buffer, 4096);
	while (bytes > 0)
	{
		write(1, buffer, bytes);
		bytes = read(fd, buffer, 4096);
	}
	close(fd);
	return (0);
}

int	main(int argc, char **argv)
{
	int	i;

	if (argc == 1)
	{
		ft_display_input();
	}
	i = 1;
	while (i < argc)
	{
		ft_display_file(argv[i]);
		i++;
	}
	return (0);
}
