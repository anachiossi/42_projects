/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tail.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 20:16:44 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/17 16:32:02 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** ************************************************************************** **
** Module:      C10 - Exercise 02                                             **
** Program:     ft_tail                                                       **
** File:        ft_tail.c                                                     **
** Allowed:     close, open, read, write, malloc, free, strerror, basename   **
**                                                                            **
** Description:                                                               **
**   Recreates the system command: tail -c N filename                        **
**   Prints the last N bytes of a file to stdout.                            **
**   Only -c option is handled. No + or - signs needed.                      **
**                                                                            **
** Usage:                                                                     **
**   ./ft_tail -c 10 filename                                                 **
**   argv[0] = "./ft_tail"                                                    **
**   argv[1] = "-c"                                                           **
**   argv[2] = "10"  (number of bytes from end)                              **
**   argv[3] = "filename"                                                     **
**                                                                            **
** Returns:                                                                   **
**   Prints last N bytes of file to stdout                                    **
**   Error messages to stderr (fd 2)                                          **
**                                                                            **
** Key concepts:                                                              **
**   - read() entire file into malloc'd buffer                                **
**   - track total bytes read                                                 **
**   - print from (total - N) onwards                                         **
**   - ft_atoi to convert argv[2] to int                                      **
** ************************************************************************** **
*/

#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int		ft_atoi(char *str);

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_print_error(char *filename)
{
	char	*file;
	char	*error;

	file = basename(filename);
	error = strerror(errno);
	write(2, "ft_tail: ", ft_strlen("ft_tail: "));
	write(2, file, ft_strlen(file));
	write(2, ": ", 2);
	write(2, error, ft_strlen(error));
	write(2, "\n", 1);
	return ;
}

int	ft_get_size(char *filename)
{
	int		fd;
	int		total;
	int		bytes;
	char	buffer[4096];

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	total = 0;
	bytes = read(fd, buffer, 4096);
	while (bytes > 0)
	{
		total += bytes;
		bytes = read(fd, buffer, 4096);
	}
	close(fd);
	return (total);
}

void	ft_tail(char *filename, int n, int size)
{
	char	*content;
	int		fd;

	content = (char *)malloc(size);
	if (!content)
		return ;
	fd = open(filename, O_RDONLY);
	read(fd, content, size);
	if (n > size)
		n = size;
	write(1, &content[size - n], n);
	close(fd);
	free(content);
	return ;
}

int	main(int argc, char **argv)
{
	char	*filename;
	int		n;
	int		size;

	if (argc < 4)
	{
		write(2, "usage: ft_tail -c N filename\n", 29);
		return (1);
	}
	filename = argv[3];
	n = ft_atoi(argv[2]);
	size = ft_get_size(filename);
	if (size == -1)
	{
		ft_print_error(filename);
		return (1);
	}
	if (size == 0)
		return (0);
	ft_tail(filename, n, size);
	return (0);
}
