/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:54:13 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/02 14:51:37 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		is_space(char a)
{
	if (a == '\f' || a == '\n' || a == '\r' ||
		a == '\t' || a == '\v' || a == ' ')
		return (1);
	return (0);
}

int		pass_spaces(char *str, int i)
{
	while (str[i] && is_space(str[i]) == 1)
		i++;
	return (i);
}

int		is_quote(char *str, int i, char quote)
{
	if (quote == 0)
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if ((i > 0) && (str[i - 1] == '\\'))
				return (0);
			return (1);
		}
	}
	else
	{
		if (str[i] == quote)
		{
			if ((i > 0) && (str[i - 1] == '\\'))
				return (0);
			return (1);
		}
	}
	return (0);
}

int		get_next_quote(char *str, int i)
{
	char tmp;

	if (!str[i] || is_quote(str, i, 0) != 1)
		return (-1);
	tmp = str[i++];
	while (str[i] && is_quote(str, i, tmp) == 0)
		i++;
	if (is_quote(str, i, tmp) == 0)
		return (-1);
	return (i);
}

int		lonely_quote(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (is_quote(str, i, 0) == 1)
		{
			if ((i = get_next_quote(str, i)) == -1)
				return (-1);
			//i -= 1;
		}
	}
	return (0);
}

int		find_string_end(char *input, int i)
{
	while (input[i] && is_space(input[i]) == 0)
	{
		if (is_quote(input, i, 0))
		{
			if ((i = get_next_quote(input, i)) == -1)
				return (-1);
		}
		i++;
	}
	return (i);
}
