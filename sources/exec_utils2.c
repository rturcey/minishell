/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 14:46:39 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/07 12:18:35 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void		handle_parent(pid_t pid, t_sh *sh)
{
	int		status;

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
	{
		while (wait(&status) != -1)
			continue ;
		g_err = (WIFEXITED(status)) ? WEXITSTATUS(status) : g_err;
	}
	else if (sh->wait == 1 || (sh->obj->pip != IS_PIPE
	&& (!sh->obj->prev || sh->obj->prev->pip != IS_PIPE)))
	{
		waitpid(pid, &status, 0);
		g_err = (WIFEXITED(status)) ? WEXITSTATUS(status) : g_err;
		sh->wait = 0;
	}
}
