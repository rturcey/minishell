/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 10:26:06 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/03 12:56:33 by esoulard         ###   ########.fr       */
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

static void	process_pipe(t_sh *sh)
{
	int status;

	if (sh->pip->pid == 0)
	{
		sh->obj->redir->cmd_output = 1;
		close(sh->pip->pipefd[1]);
		dup2(sh->pip->pipefd[0], 0);
		sh->pip->forked = 2;
		sh->pip->type = 1;
	}
	else
	{
		close(sh->pip->pipefd[0]);
		dup2(sh->pip->pipefd[1], 1);
		status = 0;
		waitpid(-1, &status, WUNTRACED);
		if (WIFEXITED(status))
			g_err = WEXITSTATUS(status);
		close(sh->pip->pipefd[1]);
		sh->pip->count--;
		exit(g_err);
	}
}

static int	pipe_process2(t_sh *sh, int *i)
{
	int		status;

	sh->pip->count++;
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
		return (1);
	}
	g_forked = 1;
	sh->pip->type = 2;
	return (0);
}

void		pipe_checks(t_sh *sh, int *i)
{
	if (sh->pip->lever == 1)
	{
		sh->pip->count++;
		if ((sh->pip->pid = fork()) == -1)
		{
			ft_dprintf(2, "fork error\n");
			exit(EXIT_FAILURE);
		}
		g_forked = 1;
		process_pipe(sh);
	}
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
		sh->obj->redir->cmd_output = sh->pip->pipefd[1];
	}
}
