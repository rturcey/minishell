/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 19:52:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/10/05 19:53:20 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		in_quotes(char *str, int i)
{
	char	tmp;
	int		j;
	int		count;

	j = i;
	count = 0;
	tmp = 0;
	while (--j > 0)
	{
		if (count == 0 && is_quote(str, j, 0) == 1)
		{
			tmp = str[j];
			count++;
		}
		else if (str[j] == tmp)
			count++;
	}
	if (count % 2 != 0)
		return (1);
	return (0);
}
