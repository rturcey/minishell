/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 15:55:09 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/12 06:54:31 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		redir_norm(int kind, t_redir *redir, char *path)
{
	if (kind == 2)
	{
		if (redir->cmd_output != 1)
			close(redir->cmd_output);
		redir->cmd_output = open(path, O_APPEND | O_CREAT | O_WRONLY, 0644);
	}
	if (kind == 1)
	{
		if (redir->cmd_output != 1)
			close(redir->cmd_output);
		redir->cmd_output = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	if (kind == 3)
	{
		if (redir->cmd_input != 0)
			close(redir->cmd_input);
		redir->cmd_input = open(path, O_RDONLY, 0644);
		return (redir->cmd_input);
	}
	return (redir->cmd_output);
}

static int		redir_err(int kind, t_redir *redir, char *path)
{
	if (kind == 2)
	{
		if (redir->err_output != 2)
			close(redir->err_output);
		redir->err_output = open(path, O_APPEND | O_CREAT | O_WRONLY, 0644);
	}
	if (kind == 1)
	{
		if (redir->err_output != 2)
			close(redir->err_output);
		redir->err_output = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	}
	return (redir->err_output);
}

static int		parse_redir(t_obj *obj, char *input, int *s_fd, int *i)
{
	char	*path;
	int		ret;

	path = NULL;
	if (s_fd[1] == 2)
		(*i)++;
	*i = pass_spaces(input, *i);
	path = sample_str(input, i, path, obj->env);
	if (s_fd[0] == 1)
		ret = redir_norm(s_fd[1], obj->redir, path);
	else
		ret = redir_err(s_fd[1], obj->redir, path);
	if (ret == -1)
	{
		maj_err(obj, ft_sprintf("bash: %s: %s\n", path, strerror(errno)), 1);
		return (print_result(obj, -1, path));
	}
	free(path);
	*i = pass_spaces(input, *i);
	return (1);
}

int				find_redir(t_obj *obj, char *input, int *i)
{
	int		j;
	int		s_fd[2];

	s_fd[0] = 1;
	*i = pass_spaces(input, *i);
	if ((s_fd[1] = is_redir(input, *i)) == 0 && ft_isdigit(input[*i]) == 0)
		return (0);
	if (ft_isdigit(input[*i]) != 0)
	{
		if ((s_fd[0] = ft_atoi(&input[*i])) > 0)
		{
			j = *i;
			while (ft_isdigit(input[*i]) != 0)
				(*i)++;
			if ((s_fd[1] = is_redir(input, *i)) == 0)
			{
				*i = j;
				return (0);
			}
		}
		else
			return (0);
	}
	(*i)++;
	return (parse_redir(obj, input, s_fd, i));
}
