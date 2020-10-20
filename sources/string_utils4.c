/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 12:19:25 by user42            #+#    #+#             */
/*   Updated: 2020/10/20 12:26:23 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		one_quote(char *s, char *s1)
{
	int	i;
	int	c;

	i = (int)ft_strlen(s) + 1;
	c = 0;
	while (--i >= 0)
	{
		if (is_quote(s, i, '\"'))
			c++;
	}
	if (c != 1)
		return (0);
	c = 0;
	i = (int)ft_strlen(s1) + 1;
	while (--i >= 0)
	{
		if (is_quote(s1, i, '\"'))
			c++;
	}
	if (c != 1)
		return (0);
	return (1);
}

int		normed_char(char c)
{
	if (!c)
		return (-1);
	if (ft_isalnum(c) == 0)
		if (c != '-' && c != '_' && c != '?')
			return (-1);
	return (0);
}
