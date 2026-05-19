/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexdump.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 14:50:24 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/18 14:50:27 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEXDUMP_H
# define FT_HEXDUMP_H

# include <errno.h>
# include <fcntl.h>
# include <libgen.h>
# include <string.h>
# include <unistd.h>

void	ft_puthex(int nbr, int digits);
void	ft_char_to_hex(unsigned char *buffer, int bytes);
void	ft_print_ascii(unsigned char *buffer, int bytes);
void	ft_print_line(unsigned char *buffer, int bytes, int offset);
void	ft_print_error(char *program, char *filename);
void	ft_memcpy(unsigned char *previous, unsigned char *buffer, int bytes);
int		ft_memcmp(unsigned char *previous, unsigned char *buffer, int bytes);
int		ft_strlen(char *str);
int		ft_open(char *program, char *filename);
void	ft_hexdump(int fd, char *program, char *filename);

#endif
