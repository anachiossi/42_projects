/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:37:04 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 15:09:13 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
** C01 - ex00
** ft_ft
** Allowed functions: None
** Key concepts: Pointers, Dereferencing
**
** Description:
** Write a function that takes a pointer to an int and sets its value to 42.
** Input: int *nbr (pointer to the integer to modify)
** Output: void
** Example: ft_ft(&n) → n == 42
*/

/*#include <stdio.h>*/

void	ft_ft(int *nbr)
{
	*nbr = 42;
}

/*int	main(void)
{
	int	a;

	a = -9239844;
	ft_ft(&a);
	printf("%d\n", a);
	return (0);
}*/
