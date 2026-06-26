/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 12:49:52 by anade-mo          #+#    #+#             */
/*   Updated: 2026/06/04 19:55:51 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_read_line(int fd, char **cache)
{
	char	*buffer;
	int		bytes;
	char	*temp;

	buffer = malloc(BUFFER_SIZE + 1);
	if (buffer == NULL)
		return (NULL);
	while (!gnl_strchr(*cache, '\n'))
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buffer), free(*cache), *cache = NULL);
		if (bytes == 0)
			return (free(buffer), *cache);
		buffer[bytes] = '\0';
		temp = *cache;
		*cache = gnl_strjoin(*cache, buffer);
		free(temp);
		if (*cache == NULL)
			return (free(buffer), NULL);
	}
	free(buffer);
	return (*cache);
}

static char	*ft_extract_line(const char *cache)
{
	char	*end;
	char	*line;
	size_t	len;

	if (gnl_strchr(cache, '\n') == NULL)
	{
		line = gnl_strdup(cache);
		return (line);
	}
	end = gnl_strchr(cache, '\n');
	len = end - cache + 1;
	line = malloc(sizeof(char) * (len + 1));
	if (line == NULL)
		return (NULL);
	gnl_memcpy(line, cache, len);
	line[len] = '\0';
	return (line);
}

static void	ft_update_cache(char **cache)
{
	char	*start;
	char	*updated;
	char	*temp;

	if (gnl_strchr(*cache, '\n') == NULL)
	{
		free(*cache);
		*cache = NULL;
		return ;
	}
	start = gnl_strchr(*cache, '\n') + 1;
	if (start[0] == '\0')
	{
		free(*cache);
		*cache = (NULL);
		return ;
	}
	updated = gnl_strdup(start);
	temp = *cache;
	*cache = updated;
	free(temp);
	return ;
}

char	*get_next_line(int fd)
{
	static char	*cache[1024];
	char		*line;

	ft_read_line(fd, &cache[fd]);
	if (cache[fd] == NULL)
		return (NULL);
	line = ft_extract_line(cache[fd]);
	if (line == NULL)
	{
		free(cache[fd]);
		cache[fd] = NULL;
		return (NULL);
	}
	ft_update_cache(&cache[fd]);
	return (line);
}
