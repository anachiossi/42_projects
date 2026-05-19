/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anade-mo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/18 19:40:29 by anade-mo          #+#    #+#             */
/*   Updated: 2026/03/23 11:32:05 by anade-mo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** C03 - ex05
** ft_strlcat
** Allowed functions: None
** Key concepts: Strings, Bounded concatenation, Buffer safety
**
** Description:
** Reproduce the behavior of strlcat (man strlcat).
** Appends src to dest, copying at most size - strlen(dest) - 1 bytes.
** Always null-terminates the result.
** Input: char *dest (destination), char *src (source), unsigned int size (total buffer size)
** Output: unsigned int (total length of the string it tried to create)
** Rules:
** - If destlen >= size, returns size + srclen (no modification to dest)
** Example: ft_strlcat("hi", " there", 5) → "hi th", returns 8
*/
//#include <unistd.h>
unsigned int	ft_strlcat(char *dest, char *src, unsigned int size)
{
	unsigned int	srclen;
	unsigned int	destlen;
	unsigned int	i;

	srclen = 0;
	destlen = 0;
	i = 0;
	while (src[srclen])
		srclen++;
	while (dest[destlen])
		destlen++;
	if (destlen >= size)
		return (size + srclen);
	while (i < (size - destlen - 1))
	{
		dest[i + destlen] = src[i];
		i++;
	}
	dest[i + destlen] = '\0';
	return (destlen + srclen);
}
/*
int	main(void)
{
	char	dest[20] = "Ana ";
	char	*src = "is a-ma-zing!";
	unsigned int	ret;
	ret = ft_strlcat(dest, src, 1);
	write(1, dest, ret);
	write(1,"\n", 1);
	return (0);
}*/
