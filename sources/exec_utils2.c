/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 14:46:39 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/17 18:12:10 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_parent(pid_t pid, int lever, t_sh *sh)
{
	int		status;

	waitpid(pid, &status, 0);
	if (lever == 1)
	{
		close(sh->obj->tube[1]);
		if (sh->obj->pip != IS_PIPE)
			close(sh->obj->prev->tube[0]);
	}
	if (sh->obj->prev && sh->obj->prev->pip == IS_PIPE)
		close(sh->obj->prev->tube[0]);
	if (WIFEXITED(status))
		g_err = WEXITSTATUS(status);
	g_forked = 0;
}
