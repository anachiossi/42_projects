/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:29:33 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 19:28:43 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long	nbr;
	char	c;

	nbr = (long)n;
	if (nbr < 0)
	{
		write(fd, "-", 1);
		nbr = -nbr;
	}
	if (nbr >= 10)
		ft_putnbr_fd((nbr / 10), fd);
	c = '0' + (nbr % 10);
	write(fd, &c, 1);
}
