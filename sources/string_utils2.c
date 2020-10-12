/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 17:19:50 by esoulard          #+#    #+#             */
/*   Updated: 2020/10/12 16:06:39 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		lonely_quote(char *str)
{
	int i;

	i = -1;
	while (str[++i])
	{
		if (is_quote(str, i, 0) == 1)
		{
			if ((i = get_next_quote(str, i, NULL)) == -1)
				return (-1);
		}
	}
	i = -1;
	while (str[++i])
	{
		if (str[i] == '|')
		{
			i = pass_spaces(str, ++i);
			if (!str[i])
				return (-1);
		}
	}
	return (0);
}

int		last_backslash(char *str)
{
	int i;
	int count;

	i = 0;
	if (!str[i])
		return (0);
	i = ft_strlen(str) - 1;
	count = 0;
	if (str[i] == '\\')
	{
		while (str[i] && str[i] == '\\')
		{
			count++;
			i--;
		}
	}
	if (count % 2 != 0)
		return (-1);
	return (0);
}

int		find_string_end(char *in, int i)
{
	while ((is_end(in, i) == 0) && is_space(in, i) == 0 &&
		is_redir(in, i) == 0)
	{
		if (is_quote(in, i, 0))
		{
			if ((i = get_next_quote(in, i, NULL)) == -1)
				return (-1);
		}
		i++;
	}
	return (i);
}

int		count_strings(char *in, int i)
{
	int count;

	count = 0;
	i = pass_spaces(in, i);
	while (is_end(in, i) == 0)
	{
		count++;
		i = find_string_end(in, i);
		if (in[i])
			++i;
		i = pass_spaces(in, i);
	}
	return (count);
}

int		find_end(char *in, int i)
{
	while (in[i] && is_end(in, i) != 1)
		i++;
	if (in[i] && is_separator(in, i) == 1)
		i++;
	return (i);
}
