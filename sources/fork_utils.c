/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 16:36:41 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/23 15:38:11 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_gfork(t_sh *sh, char *tmp)
{
	if (sh->obj->pip == IS_PIPE || (sh->obj->prev &&
		sh->obj->prev->pip == IS_PIPE))
		g_forked = IS_PIPE;
	else if (tmp && is_ms(tmp) != 0)
		g_forked = IS_EXEC;
	else if (tmp && is_ms(tmp) == 0)
		g_forked = IS_F_MS;
	else
		g_forked = IS_MS;
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
