/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 10:26:06 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/17 16:59:07 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


/*void		pipe_checks(t_sh *sh, int *i)
{
	init_pipe(sh, *i);
	if (sh->pip->lever == 1)
	{
		if (sh->pip->count == 0)
			if (pipe_process2(sh, i) == 1)
				return ;
		if (pipe(sh->pip->pipefd) == -1)
		{
			ft_dprintf(2, "pipe error\n");
			exit(EXIT_FAILURE);
		}
		sh->pip->count++;
		g_forked = 3;
		if ((sh->pip->pid = fork()) == -1)
		{
			ft_dprintf(2, "fork error\n");
			exit(EXIT_FAILURE);
		}
		process_pipe(sh, i);
	}
}*/

void	pipe_checks(t_sh *sh, int *i)
{
	int		end;

	end = find_end(sh->in, *i);
	if ((--end >= 0 && sh->in[end] == '|'))
		sh->obj->pip = IS_PIPE;
}
