/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:30 by rturcey           #+#    #+#             */
/*   Updated: 2020/08/29 13:13:17 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Haven't used it yet, but will be useful during
**further cmd parsing
*/

int		is_separator(char *str, int i)
{
	if (str[i] == ';' || str[i] == '|')
	{
		if ((i > 0) && (str[i - 1] == '\\'))
			return (0);
		return (1);
	}
	return (0);
}

int		pipeline_end(char *in, int i)
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
	return (i);
}

void	init_pipe(t_sh *sh, int i)
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

void	pipe_checks(t_sh *sh, int *i)
{
	int status;

	if (sh->pip->lever == 1)
	{
		sh->pip->count++;
		if ((sh->pip->pid = fork()) == -1)
		{
			ft_dprintf(2, "fork error\n");
			exit(EXIT_FAILURE);
		}
		if (sh->pip->pid == 0)
		{
			sh->obj->redir->cmd_output = 1;
			close(sh->pip->pipefd[1]);
			dup2(sh->pip->pipefd[0], 0);
			sh->pip->forked = 2;
			sh->pip->type = 1;
		}
		else // PARENT PROCESS
		{
			close(sh->pip->pipefd[0]);
			dup2(sh->pip->pipefd[1], 1);
			status = 0;
			//wait(&status);
			waitpid(-1, &status, WUNTRACED);
			close(sh->pip->pipefd[1]);
			sh->pip->count--;
			exit(EXIT_SUCCESS);
		}
	}
	init_pipe(sh, *i);
	if (sh->pip->lever == 1)
	{
		if (sh->pip->count == 0)
		{
			sh->pip->count++;
			if ((sh->pip->pid = fork()) == -1)
			{
				ft_dprintf(2, "fork error\n");
				exit(EXIT_FAILURE);
			}
			if (sh->pip->pid != 0)
			{
				wait(&status);
				*i = pipeline_end(sh->in, *i);
				sh->pip->type = 0;
				return ;
			}
			sh->pip->type = 2;
		}
		if (pipe(sh->pip->pipefd) == -1)
		{
			ft_dprintf(2, "pipe error\n");
			exit(EXIT_FAILURE);
		}
		sh->obj->redir->cmd_output = sh->pip->pipefd[1];
	}
}

int		general_parser(t_sh *sh)
{
	char		*sample;
	int			i;
	int			j;
	int			stock_i;


	if ((lonely_quote(sh->in) == -1) ||
		(last_backslash(sh->in) == -1))
	{
		ft_putstr_fd("bash: multi-line comments not supported\n", 2);
		return (0);
	}
	i = 0;
	j = 0;
	while (sh->in[i])
	{
		i = pass_spaces(sh->in, i);
		if ((j = parse_syntax(sh, i)) == -1)
			return (0);
		else if (j == -2)
			return (-1);
		if (!(sh->obj = obj_new(sh->env)))
			return (-1);
		if (!(sh->obj->redir = redir_new()))
			return (free_obj(sh->obj));
		sample = NULL;
		if ((sh->lev-- == 1) && (find_redir_err(sh, &i) == -1)
			&& (sh->err = 2))
				return (0);
		if ((redir_loop(sh, &i) == -1) && (sh->err = 2))
			return (-1);
		if (parse_var(sh, &i, 0) == -1)
		{
			return (free_obj(sh->obj));
		}
		pipe_checks(sh, &i);
		if (!sh->in[i])
		{
			free_obj(sh->obj);
			continue ;
		}
		stock_i = i;
		if ((sample = sample_str(sh, &i, sample)) == NULL)
			return (-1);
		if ((j = is_cmd(sample)) != -1)
		{
			init_obj(sh->obj, sample, j);
			if (sh->obj->obj == NULL)
				return (free_obj(sh->obj));
			if ((j = parse_cmds(sh, &i)) == -1)
			{
				free_obj(sh->obj);
				free_str(sample);
				return (0);
			}
			if (set_g_err(sh) == 1)
				return (free_str(sample));
			free(sample);
		}
		else
		{
			i = stock_i;
			free(sample);
			if ((j = parse_exec(sh, &i)) == -1)
				return (free_obj(sh->obj));
			else if (j == -2)
			{
				maj_err(sh, ft_sprintf("%s: command not found\n", \
				sh->obj->obj), 127);
				print_result(sh, 0, NULL);
			}
		}
		i = find_end(sh->in, i);
		if (sh->obj)
			free_obj(sh->obj);
		if (sh->pip->type == 3)
		{
			close(sh->pip->pipefd[0]);
			exit(EXIT_SUCCESS);
		}
	}
	return (0);
}
