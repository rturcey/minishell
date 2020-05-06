/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_space.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 10:49:47 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/06 11:21:12 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		is_space(char *str, int i)
{
	int count;

	count = 0;
	if (str[i] == '\f' || str[i] == '\n' || str[i] == '\r' ||
		str[i] == '\t' || str[i] == '\v' || str[i] == ' ')
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
