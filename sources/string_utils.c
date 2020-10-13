/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:54:13 by esoulard          #+#    #+#             */
/*   Updated: 2020/10/13 09:07:50 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		pass_spaces(char *str, int i)
{
	while (str[i] && is_space(str, i) == 1)
		i++;
	return (i);
}

int		check_single(char *str, int i)
{
	int j;
	int count;

	j = i;
	count = 0;
	while (--j > 0)
		if (str[j] == '\'' && is_quote(str, j, '\''))
			count++;
	if (count % 2 != 0)
		return (1);
	return (0);
}

int		is_quote2(char *str, int i, char quote, int count)
{
	if (str[i] && str[i] == quote)
	{
		if ((i > 0) && (str[i - 1] == '\\'))
		{
			if (str[i] == '\'' && str[i - 1])
				if (check_single(str, i) == 1)
					return (1);
			while (--i > 0 && str[i] == '\\')
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
		if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		{
			if ((i > 0) && (str[i - 1] == '\\'))
			{
				if (str[i] == '\'' && str[i - 1])
					if (check_single(str, i) == 1)
						return (1);
				while (--i > 0 && str[i] && str[i] == '\\')
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

int		get_next_quote(char *str, int i, t_sh *sh, int l)
{
	char tmp;

	if (!str[i])
		return (-1);
	tmp = str[i++];
	if (tmp == '\"')
	{
		if (sh)
			return (i += l);
		while (str[i] && is_quote(str, i, tmp) == 0)
			i++;
		if (!str[i] && sh)
			return (i);
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
