/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 10:10:47 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/19 12:34:48 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pass_option(t_obj *obj, char *input, int *i)
{
	while (is_end(input, *i) == 0)
	{
		if (ft_strncmp("-n", &input[*i], 2) == 0
			&& (is_end(input, *i + 2) == 1 || is_space(input, *i + 2) == 1)
			&& ((*i) += 2))
			obj->option = 1;
		else
			return;
		*i = pass_spaces(input, *i);
	}
}
