/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 19:33:36 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/18 15:33:23 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
** Module:      C10 - Exercise 03
** Program:     ft_hexdump
** File:        ft_hexdump.c
** Allowed:     close, open, read, write, malloc, free, strerror, basename
**
** Description:
**   Recreates: hexdump -C filename
**   Displays file content as hex + ASCII, 16 bytes per line.
**
** Output format (EXACT spacing matters):
**   00000000  48 65 6c 6c 6f 2c 20 57  6f 72 6c 64 21 0a  |Hello, World!.|
**   ^offset   ^8 hex bytes             ^8 hex bytes        ^ASCII
**
**   - Offset : 8 hex digits, starts at 0, +16 each line
**   - Hex    : each byte as 2 hex digits, space between each
**   - Gap    : extra space between byte 8 and byte 9
**   - Last line: pad with spaces if less than 16 bytes
**   - ASCII  : printable (32-126) as-is, everything else as '.'
**   - Each line ends with \n
**   - Final line: just the final offset, no hex or ASCII
**
** Key concepts:
**   - Read 16 bytes at a time
**   - Print offset in hex
**   - Print hex bytes with correct spacing
**   - Print ASCII column
**   - Handle last line padding
*/

#include "ft_hexdump.h"

void	ft_print_line(unsigned char *buffer, int bytes, int offset)
{
	static unsigned char	previous[16];
	static int				flag;
	static int				first_call = 1;

	if (first_call == 0 && ft_memcmp(previous, buffer, 16) == 0)
	{
		if (flag == 0)
		{
			write(1, "*\n", 2);
			flag = 1;
		}
		return ;
	}
	first_call = 0;
	flag = 0;
	ft_puthex(offset, 8);
	ft_char_to_hex(buffer, bytes);
	ft_print_ascii(buffer, bytes);
	ft_memcpy(previous, buffer, 16);
}

void	ft_hexdump(int fd, char *program, char *filename)
{
	unsigned char	buffer[16];
	int				bytes;
	int				offset;

	offset = 0;
	bytes = read(fd, buffer, 16);
	if (bytes <= 0)
	{
		if (bytes == -1)
			ft_print_error(program, filename);
		return ;
	}
	while (bytes > 0)
	{
		ft_print_line(buffer, bytes, offset);
		offset += bytes;
		bytes = read(fd, buffer, 16);
	}
	ft_puthex(offset, 8);
	write(1, "\n", 1);
	return ;
}

int	ft_open(char *program, char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_print_error(program, filename);
		return (1);
	}
	return (fd);
}

int	main(int argc, char **argv)
{
	int	fd;
	int	i;

	i = 1;
	if (argc == 1 || (argc == 2 && argv[1][0] == '-'))
	{
		fd = 0;
		ft_hexdump(fd, argv[0], "");
		return (0);
	}
	while (i < argc)
	{
		if (argv[i][0] == '-')
			i++;
		else
		{
			fd = ft_open(argv[0], argv[i]);
			ft_hexdump(fd, argv[0], argv[i]);
			close(fd);
			i++;
		}
	}
	return (0);
}
