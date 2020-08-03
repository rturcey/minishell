/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 20:25:27 by esoulard          #+#    #+#             */
/*   Updated: 2020/08/03 18:42:41 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*new;
	size_t	i;

	i = 0;
	if (!s)
		return (NULL);
	if (!(new = malloc(sizeof(char) * (len + 1))))
		return (NULL);
	if (start > ft_pstrlen(s))
	{
		new[0] = '\0';
		return (new);
	}
	while (s[start] && i < len)
		new[i++] = s[start++];
	new[i] = '\0';
	return (new);
}
