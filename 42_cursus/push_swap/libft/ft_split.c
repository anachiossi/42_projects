/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 14:27:49 by anade-mo          #+#    #+#             */
/*   Updated: 2026/05/23 12:30:36 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_word_count(char const *s, char c)
{
	size_t	i;
	size_t	count;
	int		in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == c)
			in_word = 0;
		else if (s[i] != c && in_word == 0)
		{
			count++;
			in_word = 1;
		}
		i++;
	}
	return (count);
}

static size_t	ft_word_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static char	**free_all(char **arr, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

static char	**ft_alloc_split(size_t word_count)
{
	char	**split;

	split = malloc(sizeof(char *) * (word_count + 1));
	if (split == NULL)
		return (NULL);
	return (split);
}

char	**ft_split(char const *s, char c)
{
	char	**split;
	size_t	word_len;
	size_t	word_count;
	size_t	i;
	size_t	j;

	if (s == NULL)
		return (NULL);
	i = 0;
	j = 0;
	word_count = ft_word_count(s, c);
	split = ft_alloc_split(word_count);
	while (j < word_count)
	{
		while (s[i] == c)
			i++;
		word_len = ft_word_len(s + i, c);
		split[j] = ft_substr(s, i, word_len);
		if (split[j] == (NULL))
			return (free_all(split, j));
		i = i + word_len;
		j++;
	}
	split[j] = NULL;
	return (split);
}
