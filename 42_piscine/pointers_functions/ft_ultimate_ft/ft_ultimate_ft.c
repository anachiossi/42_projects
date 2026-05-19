/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ultimate_ft.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/16 13:38:39 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/16 15:32:17 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C01 - ex01
** ft_ultimate_ft
** Allowed functions: None
** Key concepts: Pointers, Multiple indirection levels
**
** Description:
** Write a function that takes a 9-level pointer to an int and sets its value
** to 42.
** Input: int *********nbr (9-level pointer to the integer to modify)
** Output: void
** Example: ft_ultimate_ft(&&&&&&&&&n) → n == 42
*/

/*#include <stdio.h>*/

void	ft_ultimate_ft(int *********nbr)
{
	*********nbr = 42;
}

/*int	main(void)
{
	int	a;
	int	*pointer1 = &a;
	int	**pointer2 = &pointer1;
	int     ***pointer3 = &pointer2;
	int     ****pointer4 = &pointer3;
	int     *****pointer5 = &pointer4;
	int     ******pointer6 = &pointer5;
	int     *******pointer7 = &pointer6;
	int     ********pointer8 = &pointer7;
	int     *********pointer9 = &pointer8;

	a = -391823;
	ft_ultimate_ft(pointer9);
	printf("%d\n", a);
	return (0);
}*/
