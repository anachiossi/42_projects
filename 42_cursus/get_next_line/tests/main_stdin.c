/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_stdin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 14:42:30 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/05 16:17:34 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	char	*line;

	printf("Stdin test — type the lines, press Enter after each.\n");
	printf("Ctrl+D to stop.\n");
	while ((line = get_next_line(0)) != NULL)
	{
		printf("got: %s", line);
		free(line);
	}
	return (0);
}
