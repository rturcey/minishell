/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 16:36:41 by esoulard          #+#    #+#             */
/*   Updated: 2020/10/10 12:00:46 by esoulard         ###   ########.fr       */
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
	if (sh->obj->pip != IS_PIPE)
		g_last = 1;
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

void	kill_pipe(int sig)
{
	int status;

	while (wait(&status) != -1)
		kill(g_pid, sig);
	g_pid = -1;
}
