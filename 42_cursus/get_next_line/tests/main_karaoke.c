/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_karaoke.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:16:55 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/05 16:17:23 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int		fd1;
	char	*line;

	printf("🎸 Karaoke test of Get Next Line 🎸\n");
	printf("Ready to start? press ENTER:\n\n");
	getchar();
	fd1 = open("tests/sing_along.txt", O_RDONLY);
	if (fd1 == -1)
	{
		perror("tests/sing_along.txt");
		return (1);
	}
	while ((line = get_next_line(fd1)) != NULL)
	{
		printf("%s", line);
		free(line);
		getchar();
	}
	close(fd1);
	printf("\n🎤 Thanks and have fun! 🎤\n");
	return (0);
}
