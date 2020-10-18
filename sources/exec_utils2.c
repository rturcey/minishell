/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 14:46:39 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/18 13:29:22 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void		wait_pipeline(pid_t pid, t_sh *sh)
{
	int ret;
	int status;

	while ((ret = wait(&status)) != -1)
	{
		if (ret == pid)
		{
			while (!WIFEXITED(status))
				if (!WIFSIGNALED(status) || sh->obj->pip == IS_PIPE)
					break ;
			if (WIFEXITED(status) && sh->obj->pip != IS_PIPE)
				g_err = WEXITSTATUS(status);
		}
	}
	g_pid = -1;
}

void		handle_parent(pid_t pid, t_sh *sh)
{
	int		status;

	g_pid = pid;
	status = 0;
	if (sh->obj->prev && sh->obj->prev->pip == IS_PIPE)
	{
		if (sh->obj->prev->tube[0] != -1)
			close(sh->obj->prev->tube[0]);
		if (sh->obj->prev->tube[1] != -1)
			close(sh->obj->prev->tube[1]);
	}
	if (sh->obj->pip == IS_PIPE && (sh->obj->type == 3 || sh->obj->type == 5))
		sh->wait = 1;
	if (sh->wait == 0 && sh->obj->pip != IS_PIPE
	&& sh->obj->prev && sh->obj->prev->pip == IS_PIPE)
		wait_pipeline(pid, sh);
	else if (sh->wait == 1 || (sh->obj->pip != IS_PIPE
	&& (!sh->obj->prev || sh->obj->prev->pip != IS_PIPE)))
	{
		waitpid(pid, &status, 0);
		g_err = (WIFEXITED(status)) ? WEXITSTATUS(status) : g_err;
		sh->wait = 0;
	}
}
