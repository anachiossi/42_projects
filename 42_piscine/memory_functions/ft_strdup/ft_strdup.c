/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 10:09:12 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/31 10:09:32 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C07 - ex00
** ft_strdup
** Allowed functions: malloc
** Key concepts: Memory allocation, Strings, Heap
**
** Description:
** Write a function that duplicates a string by allocating enough memory
** with malloc, copying the string into it, and returning the new pointer.
** Input: char *str (the string to duplicate)
** Output: char * (pointer to the newly allocated copy, or NULL on failure)
** Example: ft_strdup("hello") → a new heap-allocated "hello"
*/

#include <stdlib.h>
//#include <stdio.h>
//#include <unistd.h>

int	ft_strlen(char *str)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (str[i])
	{
		len++;
		i++;
	}
	return (len);
}

char	*ft_strdup(char *str)
{
	int	size;
	int	i;
	char	*copy;

	i = 0;
	size = ft_strlen(str) + 1;
	copy = (char *)malloc(size);
	if (copy == NULL)
		return NULL;
	while (str[i])
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

/*
int	main(void)
{
    char *result;
    int  i;

    i = 0;
    result = ft_strdup("a maloca do maloca deu a loca, oloco meu");
    while (result[i])
    {
        write(1, &result[i], 1);
        i++;
    }
    write(1, "\n", 1);
    free(result);
    return (0);
}*/
