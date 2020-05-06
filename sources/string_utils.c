/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:54:13 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/06 10:49:07 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		pass_spaces(char *str, int i)
{
	while (str[i] && is_space(str[i]) == 1)
		i++;
	return (i);
}

int		is_quote2(char *str, int i, char quote, int count)
{
	if (str[i] == quote)
	{
		if ((i > 0) && (str[i - 1] == '\\'))
		{
			while (str[--i] && str[i] == '\\')
				count++;
			if (count % 2 != 0)
				return (0);
		}
		return (1);
	}
	return (0);
}

int		is_quote(char *str, int i, char quote)
{
	int count;

	count = 0;
	if (quote == 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if ((i > 0) && (str[i - 1] == '\\'))
			{
				while (str[--i] && str[i] == '\\')
					count++;
				if (count % 2 != 0)
					return (0);
			}
			return (1);
		}
	}
	else
		return (is_quote2(str, i, quote, count));
	return (0);
}

int		get_next_quote(char *str, int i)
{
	char tmp;

	if (!str[i])
		return (-1);
	tmp = str[i++];
	if (tmp == '\"')
	{
		while (str[i] && is_quote(str, i, tmp) == 0)
			i++;
		if (is_quote(str, i, tmp) == 0)
			return (-1);
	}
	else if (tmp == '\'')
	{
		while (str[i] && str[i] != tmp)
			i++;
		if (!str[i])
			return (-1);
	}
	return (i);
}
