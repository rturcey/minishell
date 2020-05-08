/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/02 15:55:09 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/08 09:13:07 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		parse_redir(t_redir *redir, char *input, int *i, int *s_fd)
{
	char	*path;

	path = NULL;
	if (s_fd[1] == 2)
		(*i)++;
	*i = pass_spaces(input, *i);
	path = sample_str(input, i, path);
	if (s_fd[0] == 1)
	{
		if (s_fd[1] == 2 && (redir->cmd_output_pos = ">>"))
		{
			if (redir->cmd_output != 1)
				close(redir->cmd_output);
			redir->cmd_output = open(path, O_APPEND | O_CREAT | O_WRONLY, 0644);
		}
		if (s_fd[1] == 1 && (redir->cmd_output_pos = ">"))
		{
			if (redir->cmd_output != 1)
				close(redir->cmd_output);
			redir->cmd_output = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
		if (s_fd[1] == 3)
		{
			if (redir->cmd_input != 0)
				close(redir->cmd_input);
			redir->cmd_input = open(path, O_CREAT | O_RDONLY, 0644);
		}
	}
	else
	{
		if (s_fd[1] == 2 && (redir->err_output_pos = ">>"))
		{
			if (redir->err_output != 2)
				close(redir->err_output);
			redir->err_output = open(path, O_APPEND | O_CREAT | O_WRONLY, 0644);
		}
		if (s_fd[1] == 1 && (redir->err_output_pos = ">"))
		{
			if (redir->err_output != 2)
				close(redir->err_output);
			redir->err_output = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		}
	}
	free(path);
	*i = pass_spaces(input, *i);
	return (1);

}

int				find_redir(t_redir *redir, char *input, int *i)
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
	return (parse_redir(redir, input, i, s_fd));
}
