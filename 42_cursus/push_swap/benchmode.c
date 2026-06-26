/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmode.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <anade-mo@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 12:29:30 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/21 23:42:36 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	bench_s(int *n)
{
	ft_putstr_fd("[bench] sa: ", 2);
	ft_putnbr_fd(n[0], 2);
	ft_putstr_fd("\tsb: ", 2);
	ft_putnbr_fd(n[1], 2);
	ft_putstr_fd("\tss: ", 2);
	ft_putnbr_fd(n[2], 2);
	ft_putstr_fd("\tpa: ", 2);
	ft_putnbr_fd(n[3], 2);
	ft_putstr_fd("\tpb: ", 2);
	ft_putnbr_fd(n[4], 2);
	ft_putchar_fd('\n', 2);
}

static void	bench_r(int *n)
{
	ft_putstr_fd("[bench] ra: ", 2);
	ft_putnbr_fd(n[5], 2);
	ft_putstr_fd("\trb: ", 2);
	ft_putnbr_fd(n[6], 2);
	ft_putstr_fd("\trr: ", 2);
	ft_putnbr_fd(n[7], 2);
	ft_putstr_fd("\trra: ", 2);
	ft_putnbr_fd(n[8], 2);
	ft_putstr_fd("\trrb: ", 2);
	ft_putnbr_fd(n[9], 2);
	ft_putstr_fd("\trrr: ", 2);
	ft_putnbr_fd(n[10], 2);
	ft_putchar_fd('\n', 2);
}

void	benchmode(float d, int *n, int level)
{
	char	*s[4];

	s[0] = "adaptive sort";
	s[1] = "selection / O(n²)";
	s[2] = "chunk / O(n√n)";
	s[3] = "radix / O(n log n)";
	ft_putstr_fd("[bench] disorder: ", 2);
	ft_putfloat_fd(d * 100, 2, 2);
	ft_putstr_fd("%\n[bench] strategy: ", 2);
	ft_putstr_fd(s[level], 2);
	ft_putstr_fd("\n[bench] total ops: ", 2);
	ft_putnbr_fd(n[11], 2);
	ft_putchar_fd('\n', 2);
	bench_s(n);
	bench_r(n);
}
