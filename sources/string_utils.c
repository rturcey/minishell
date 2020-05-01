/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:54:13 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/01 15:54:06 by esoulard         ###   ########.fr       */
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

int		is_quote(char *str, int i)
{
	if (str[i] == '\'' || str[i] == '\"')
	{
		if ((i > 0) && (str[i - 1] == '\\'))
			return (0);
		return (1);
	}
	return (0);
}

int		lonely_quote(char *str)
{
	int i;
	int count;

	i = -1;
	count = 0;
	while (str[++i])
	{
		if (is_quote(str, i) == 1)
			count++;
	}
	if (count % 2 != 0)
		return (1);
	return (0);
}

int		find_string_end(char *input, int i)
{
	char	tmp;

	while (input[i] && is_space(input[i]) == 0)
	{
		if (is_quote(input, i))
		{
			tmp = input[i++];
			while (input[i] && input[i] != tmp)
				i++;
		}
		i++;
	}
	return (i);
}
