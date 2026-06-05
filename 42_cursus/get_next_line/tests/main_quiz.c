/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_quiz.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:50:55 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/05 19:02:57 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int	main(void)
{
	int		fd;
	char	*answer;
	char	*correct;

	fd = open("tests/sing_along.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("tests/sing_along.txt");
		return (1);
	}
	printf("\n\n🎸 Bohemian Rhapsody Quiz 🎸\n\n");
	printf("Fun interactive way to see if your get next line bonus worked.\n");
	printf("Do you remember the lyrics? Type the first verse and hit enter.\n\n");
	while ((correct = get_next_line(fd)) != NULL)
	{
		answer = get_next_line(0);
		if (answer == NULL)
			break ;
		if (strcmp(answer, correct) == 0)
			printf("✅\n");
		else
			printf("❌  is: %s\n", correct);
		free(answer);
		free(correct);
	}
	close(fd);
	return (0);
}
