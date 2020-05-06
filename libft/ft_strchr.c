/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 12:43:00 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/06 10:56:49 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	char	*haystack;
	char	needle;
	int		i;

	haystack = (char *)s;
	needle = (char)c;
	i = -1;
	while (haystack[++i])
	{
		if (haystack[i] == needle)
			return ((char *)&s[i]);
	}
	if (needle == '\0' && haystack[i] == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

char	*ft_strspchr(const char *s)
{
	int		i;

	i = -1;
	while (s[++i])
		if (is_space(s[i]) == 1)
			return ((char *)&s[i]);
	return (NULL);
}
