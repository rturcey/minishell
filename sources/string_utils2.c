/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/05 17:19:50 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/13 00:06:33 by esoulard         ###   ########.fr       */
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
			if ((i = get_next_quote(str, i)) == -1)
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
		return (-1);
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

int		find_string_end(char *input, int i)
{
	while (input[i] && is_space(input, i) == 0)//replace input[i] with is_end?
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

int		count_strings(char *input, int i)
{
	int count;

	count = 1;
	i = pass_spaces(input, i);
	while (is_end(input, i) == 0)
	{
		count++;
		i = find_string_end(input, i);
		i = pass_spaces(input, i);
	}
	return (count);
}
