/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_display_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/12 22:21:03 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/12 22:25:39 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C10 - ex00
** ft_display_file
** Allowed functions: open, read, write, close
** Key concepts: File I/O, Error handling, System calls
**
** Description:
** Write a program that displays the content of a file passed as an argument.
** Errors are written to stderr.
** Input: argv[1] (the filename to read)
** Output: file content to stdout, error messages to stderr
** Rules:
** - Exactly one argument must be provided
** - Prints "File name missing." if no argument
** - Prints "Too many arguments." if more than one argument
** - Prints "Cannot read file." if the file cannot be opened
*/

#include <fcntl.h>
#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_display_file(char *filename)
{
	int		fd;
	int		bytes;
	char	buffer[4096];

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
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
	if (argc == 1)
	{
		write(2, "File name missing.\n", ft_strlen("File name missing.") + 1);
		return (1);
	}
	if (argc > 2)
	{
		write(2, "Too many arguments.\n", ft_strlen("Too many arguments.") + 1);
		return (1);
	}
	if (argc == 2)
	{
		if (ft_display_file(argv[1]) < 0)
		{
			write(2, "Cannot read file.\n", ft_strlen("Cannot read file.") + 1);
			return (1);
		}
	}
	return (0);
}
