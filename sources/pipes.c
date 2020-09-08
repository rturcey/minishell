/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 10:26:06 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/08 14:54:38 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	pipeline_end(char *in, int i)
{
	while (in[i] && in[i] != ';')
	{
		if (is_quote(in, i, 0))
		{
			if ((i = get_next_quote(in, i)) == -1)
				return (-1);
		}
		i++;
	}
	if (in[i] && i > 0 && in[i - 1] == '\\')
		pipeline_end(in, i);
	if (in[i] == ';')
		++i;
	return (i);
}

static void	init_pipe(t_sh *sh, int i)
{
	int	end;

	if (sh->in[i])
	{
		end = find_end(sh->in, i);
		if ((--end >= 0 && sh->in[end] == '|'))
			sh->pip->lever = 1;
		else
		{
			sh->pip->lever = 0;
			if (sh->pip->count > 0)
				sh->pip->type = 3;
		}
	}
	else
	{
		sh->pip->lever = 0;
		if (sh->pip->count > 0)
			sh->pip->type = 3;
	}
}

/*
** 4 (then 1) : first child : 0 has been duped. No pipe before. Pipe after
** 4 (then 3) : CHILD last of pipeline : 0 has been duped. Pipe before.
** 				No pipe after
** 2 : child now parent = 1 has been duped. Pipe before and pipe after.
** 0 : main or nothing, nothing duped. No pipe before or after
*/

static void	process_pipe(t_sh *sh, int *i)
{
	int end;

	g_forked = 3;
	if (sh->pip->pid == 0)
	{
		g_err = 0;
		*i = find_end(sh->in, *i);
		end = find_end(sh->in, *i);
		sh->obj->redir->cmd_output = 1;
		close(sh->pip->pipefd[1]);
		dup2(sh->pip->pipefd[0], 0);
		sh->obj->redir->cmd_in = 0;
		sh->pip->forked = 2;
		sh->pip->type = 4;
	}
	else
	{
		if (sh->pip->count > 1)
			sh->pip->type = 2;
		close(sh->pip->pipefd[0]);
		sh->obj->redir->cmd_output = 1;
		dup2(sh->pip->pipefd[1], 1);
	}
}

static int	pipe_process2(t_sh *sh, int *i)
{
	int		status;

	if ((sh->pip->pid = fork()) == -1)
	{
		ft_dprintf(2, "fork error\n");
		exit(EXIT_FAILURE);
	}
	if (sh->pip->pid != 0)
	{
		wait(&status);
		if (WIFEXITED(status))
			g_err = WEXITSTATUS(status);
		*i = pipeline_end(sh->in, *i);
		sh->pip->type = 0;
		g_forked = 0;
		return (1);
	}
	sh->pip->type = 1;
	return (0);
}

void		pipe_checks(t_sh *sh, int *i)
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
}
