/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rev_int_tab.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:46:30 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 18:56:35 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
** C01 - ex07
** ft_rev_int_tab
** Allowed functions: None
** Key concepts: Arrays, Two-pointer technique, In-place reversal
**
** Description:
** Write a function that reverses an integer array in place.
** The first element becomes the last and vice versa.
** Input: int *tab (pointer to the array), int size (number of elements)
** Output: void
** Example: {1, 2, 3} → {3, 2, 1}
*/

void	ft_rev_int_tab(int *tab, int size)
{
	int	left;
	int	right;
	int	hold;

	left = 0;
	right = size - 1;
	while (left < right)
	{
		hold = tab[left];
		tab[left] = tab[right];
		tab[right] = hold;
		left++;
		right--;
	}
}
