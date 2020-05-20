/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 16:28:32 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/20 17:44:41 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_numstr(char *str)
{
	int i;

	i = -1;
	if (!str || str[0] == '\0')
		return (0);
	if (str[0] == '-' || str[0] == '+')
		i = 0;
	while (str[++i])
	{
		if (ft_isdigit((int)str[i]) == 0)
			return (0);
	}
	return (1);
}
