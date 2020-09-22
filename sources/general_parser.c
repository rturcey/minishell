/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:30 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/22 10:51:52 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Haven't used it yet, but will be useful during
**further cmd parsing
*/

int			is_separator(char *str, int i)
{
	if (str[i] == ';' || str[i] == '|')
	{
		if ((i > 0) && (str[i - 1] == '\\'))
			return (0);
		return (1);
	}
	return (0);
}

static int	first_checks(t_sh *sh, int *i)
{
	int		j;

	*i = pass_spaces(sh->in, *i);
	if ((j = parse_syntax(sh, *i)) == -1 && free_obj(&sh->obj) == -1)
		return (0);
	else if (j == -2)
		return (-1);
	if (!(sh->obj = obj_new(sh->env, sh->obj)))
		return (-1);
	if (!(sh->obj->redir = redir_new()))
		return (free_obj(&sh->obj));
	pipe_checks(sh, i);
	if ((sh->lev-- == 1) && (find_redir_err(sh, i) == -1)
	&& (g_err = 2) && free_obj(&sh->obj) == -1)
		return (0);
	if ((redir_loop(sh, i) == -1) && (g_err = 2))
		return (-1);
	if (parse_var(sh, i, 0) == -1)
		return (free_obj(&sh->obj));
	return (1);
}

static int	parse_sample(t_sh *sh, int *i, int stock, char *sample)
{
	int		j;

	if ((j = is_cmd(sample)) != -1)
	{
		if (!(init_obj(sh->obj, sample, j)))
			return (free_obj(&sh->obj));
		if (sh->obj->pip == IS_PIPE ||
		(sh->obj->prev && sh->obj->prev->pip == IS_PIPE))
		{
			if ((j = try_exec(NULL, sh, i)) == -1)
				return (0);
		}
		else if ((j = parse_cmds(sh, i)) == -1)
			return (0);
		if (set_g_err(sh) == 1)
			return (-1);
	}
	else
	{
		*i = stock;
		if ((j = parse_exec(sh, i)) == -1)
			return (free_obj(&sh->obj));
	}
	return (1);
}

static int	general_loop(t_sh *sh, int *i)
{
	int		j;
	int		stock;
	char	*sample;

	sample = NULL;
	stock = *i;
	*i = pass_spaces(sh->in, *i);
	if (!(sample = sample_str(sh, i, sample)))
		return (-1);
	if (!sample[0])
		free_obj(&sh->obj);
	else if ((j = parse_sample(sh, i, stock, sample)) != 1
	&& free_str(sample))
		return (j);
	free(sample);
	*i = find_end(sh->in, *i);
	return (1);
}

int			general_parser(t_sh *sh)
{
	int			i;
	int			j;

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
		if ((j = first_checks(sh, &i)) != 1)
			return (j);
		if (!sh->in[i])
		{
			free_obj(&sh->obj);
			continue ;
		}
		if ((j = general_loop(sh, &i)) != 1)
			return (j);
	}
	return (0);
}
