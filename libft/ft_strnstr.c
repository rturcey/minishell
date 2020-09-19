/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 13:18:22 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/19 09:29:28 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	char	*haycast;

	haycast = (char *)haystack;
	i = 0;
	if (!needle[0])
		return (haycast);
	while (haycast[i] && i < len)
	{
		j = 0;
		while (haycast[i + j] && needle[j]
			&& haycast[i + j] == needle[j] && (i + j) < len)
			j++;
		if (!(needle[j]))
			return (&haycast[i]);
		i++;
	}
	return (NULL);
}

char	*ft_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;
	char	*haycast;

	if (!haystack)
		return (NULL);
	haycast = (char *)haystack;
	i = 0;
	if (!needle[0])
		return (haycast);
	while (haycast[i])
	{
		j = 0;
		while (haycast[i + j] && needle[j]
			&& haycast[i + j] == needle[j])
			j++;
		if (!(needle[j]))
			return (&haycast[i]);
		i++;
	}
	return (NULL);
}
