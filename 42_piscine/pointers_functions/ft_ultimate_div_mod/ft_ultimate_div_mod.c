/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_div_mod.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:42:44 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 16:53:11 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C01 - ex04
** ft_ultimate_div_mod
** Allowed functions: None
** Key concepts: Pointers, Integer division, In-place modification
**
** Description:
** Write a function that divides *a by *b, storing the quotient back in *a
** and the remainder back in *b.
** Input: int *a, int *b (pointers to the operands, modified in place)
** Output: void
** Example: ft_ultimate_div_mod(&a, &b) where a=42, b=5 → a == 8, b == 2
*/

void	ft_ultimate_div_mod(int *a, int *b)
{
	int temporary;

	temporary = *a / *b;
	*b = *a % *b;
	*a = temporary;
}
