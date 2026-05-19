/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sort_int_tab.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:47:52 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 20:10:58 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C01 - ex08
** ft_sort_int_tab
** Allowed functions: None
** Key concepts: Arrays, Sorting, Bubble sort
**
** Description:
** Write a function that sorts an integer array in ascending order
** using a bubble sort algorithm.
** Input: int *tab (pointer to the array), int size (number of elements)
** Output: void
** Example: {3, 1, 2} → {1, 2, 3}
*/
void	ft_sort_int_tab(int *tab, int size)
{
	int	index;
	int	hold;
	int	restart;

	restart = 0;
	while (restart < size)
	{
		index = 0;
		while (index < (size -1))
		{
			if (tab[index] > tab[index + 1])
			{
				hold = tab[index];
				tab[index] = tab[index + 1];
				tab[index + 1] = hold;
			}
			index++;
		}
		restart++;
	}
}
