/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 20:08:07 by anade-mo          #+#    #+#             */
/*   Updated: 2026/04/11 20:08:16 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

//#include <unistd.h>

int	ft_is_separator(char c, char *charset)
{
	int	i;

	i = 0;
	while (charset[i])
	{
		if (c == charset[i])
			return (1);
		else
			i++;
	}
	return (0);
}

int	ft_word_count(char *str, char *charset)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str[i])
	{
		if (ft_is_separator(str[i], charset) == 1)
			in_word = 0;
		else if (in_word == 0)
		{
			count++;
			in_word = 1;
		}
		i++;
	}
	return (count);
}

int	ft_word_len(char *str, char *charset, int start)
{
	int	word_len;

	word_len = 0;
	while (str[start] && ft_is_separator(str[start], charset) == 0)
	{
		word_len++;
		start++;
	}
	return (word_len);
}

char	*ft_extract_word(char *str, int start, int len)
{
	char	*word;
	int		i;

	i = 0;
	word = (char *)malloc((len + 1) * sizeof(char));
	while (i < len)
	{
		word[i] = str[start];
		i++;
		start++;
	}
	word[i] = '\0';
	return (word);
}

char	**ft_split(char *str, char *charset)
{
	char	**split;
	int		i;
	int		j;
	int		start;
	int		len;

	split = (char **)malloc(sizeof(char *) * (ft_word_count(str, charset) + 1));
	j = 0;
	i = 0;
	while (str[i] && j < ft_word_count(str, charset))
	{
		if (ft_is_separator(str[i], charset) == 1)
			i++;
		else
		{
			start = i;
			len = ft_word_len(str, charset, start);
			split[j] = ft_extract_word(str, start, len);
			j++;
			i += len;
		}
	}
	split[j] = NULL;
	return (split);
}
/*
int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	main(int argc, char **argv)
{
	char	**result;
	int		i;

	if (argc != 3)
	{
		write(1, "usage: ./ft_split <string> <charset>\n", 37);
		return (1);
	}
	result = ft_split(argv[1], argv[2]);
	i = 0;
	while (result[i])
	{
		write(1, result[i], ft_strlen(result[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}*/
