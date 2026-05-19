/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_div_mod.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:41:39 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 16:26:42 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C01 - ex03
** ft_div_mod
** Allowed functions: None
** Key concepts: Pointers, Integer division, Modulo
**
** Description:
** Write a function that divides a by b, storing the quotient in the integer
** pointed to by div and the remainder in the integer pointed to by mod.
** Input: int a, int b (operands), int *div, int *mod (output pointers)
** Output: void
** Example: ft_div_mod(42, 5, &d, &m) → d == 8, m == 2
*/
/*#include <stdio.h>*/

void	ft_div_mod(int a, int b, int *div, int *mod)
{
	*div = a / b;
	*mod = a % b;
}

/*int	main(void)
{
	int	div;
	int	mod;

	ft_div_mod(42, 5, &div, &mod);
	printf("div: %d, mod: %d\n", div, mod);
	return (0);
}*/
