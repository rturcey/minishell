/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 10:03:58 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/11 12:41:59 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		check_empty_var(t_sh *sh, int *i)
{
	if (!sh->obj->obj[0] && sh->obj->qts == 0)
	{
		free(sh->obj->obj);
		sh->obj->obj = NULL;
		*i = pass_spaces(sh->in, *i);
		g_err = 0;
		if (sh->in[*i])
		{
			if (sh->in[*i] && is_end(sh->in, *i))
				(*i)++;
			pipe_checks(sh, i);
		}
		return (1);
	}
	return (0);
}

void	split_pass_quotes(char const *s, int *i)
{
	if (!s[*i] || is_quote((char *)s, *i, 0) == 0)
		return ;
	if (s[*i] == '\"' && ++(*i))
		while (s[*i] && s[*i] != '\"')
			(*i)++;
	else if (s[*i] == '\'' && ++(*i))
		while (s[*i] && s[*i] != '\'')
			(*i)++;
	if (s[*i])
		(*i)++;
}

int		start_exec(char *tmp, t_sh *sh, int *i)
{
	if (dup_exec(sh) == -1)
		return (-1);
	if (!tmp)
	{
		if (parse_cmds(sh, i) == -1)
			return (-1);
		exit(g_err);
	}
	else
	{
		execve(tmp, sh->obj->args, sh->obj->charenv);
		maj_err(sh, ft_sprintf("bash: %s: %s\n", sh->obj->obj, \
			strerror(errno)), 127);
		exit(print_result(sh, g_err, NULL));
	}
	return (0);
}

int		free_shrun_arr(t_sh *sh, int ret)
{
	int i;

	if (!(sh->running))
		return (-1);
	i = -1;
	while (++i < sh->running_nb)
	{
		free(sh->running[i]);
		sh->running[i] = NULL;
	}
	free(sh->running);
	sh->running = NULL;
	sh->running_nb = 0;
	return (ret);
}

int		only_n(char *str, int i)
{
	if (str[i++] != '-')
		return (0);
	while (is_space(str, i) == 0 &&
		is_end(str, i) == 0)
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
