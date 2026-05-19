/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/30 16:23:28 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/31 10:58:26 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*c07  *ex03,
 * ft_strjoin.c
 * function malloc
 *  concatenatees all strings pointed to by strs, separated by sep.
 * * size is the number of strings in strs.
 * if seize is 0, return empty that can be freed using free();
 *
 */

#include <stdlib.h>
#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while(str[i])
	{
		len++;
		i++;
	}
	return (len);
}

int	ft_strslens(int count, char **str)
{
	int	sum_lens;
	int	i;
	int	j;

	i = 0;
	sum_lens = 0;
	while (i < count)
	{
		j = 0;
		while (str[i][j])
		{
			sum_lens++;
			j++;
		}
		i++;
	}
	return (sum_lens);
}

char	*ft_concat(int size, char **strs, char *sep, char *result)
{
	int	i;
	int	j;
	int	k;
	char	*concat;
	
	i = 0;
        k = 0;
        concat = result;
	while (i < size)
        {   
                j = 0;
                while(strs[i][j])
                        concat[k++] = strs[i][j++];
                if (i < size - 1)
                {
                        j = 0;
                        while (sep[j])
                                concat[k++] = sep[j++];
                }
                i++;
        }
        concat[k] = '\0';
        return (concat);
}

char	*ft_strjoin(int size, char **strs, char *sep)
{
	char	*result;
	int	total_len;
	
	total_len = (ft_strlen(sep)*(size - 1)) + (ft_strslens(size, strs) + 1);
	result = (char*)malloc(sizeof(char) * (total_len));
	if (result == NULL)
		return NULL;
	if (size == 0)
                return (result);
	result = ft_concat(size, strs, sep, result);
	return (result);
}
/*
int	main(int argc, char **argv)
{
	char	*concat;
	int	i;

	i = 0;
	if (argc < 2)
	{
		write(1, "Error", 5);
		return (1);
	}
	concat = ft_strjoin(argc - 1, argv + 1, argv [argc - 1]);
	while (concat[i])
	{
		write(1, &concat[i], 1);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}
*/
