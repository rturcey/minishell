/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 16:36:41 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/19 09:01:47 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_gfork(char *tmp)
{
	if (!tmp)
		g_forked = 1;
	else if (g_forked != 3)
		g_forked = (is_ms(tmp) == 0) ? 2 : 1;
}

int		is_ms(char *exec)
{
	int i;

	i = ft_strlen(exec) - 1;
	while (exec[i] && exec[i] != '/')
		i--;
	if (i >= 0 && exec[i] == '/')
		i++;
	if (ft_strncmp(&exec[i], "minishell",
		ft_strlen(&exec[i])) == 0)
		return (0);
	return (1);
}
