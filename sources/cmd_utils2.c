/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:29:07 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/06 10:52:36 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**DUMMY PARSING FUNCTIONS
*/

int		parse_pwd(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*workdir;

	workdir = NULL;
	(void)env;
	while (input[*i] && is_separator(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		if (find_redir(obj->redir, input, i) == 0)
		{
			while (*i < (int)ft_strlen(input))
				if (find_redir(obj->redir, input, i) == 0)
					(*i)++;
			ft_putstr_fd("pwd: too many arguments\n", obj->redir->err_output);
			return (-1);
		}
	}
	if (!(workdir = ft_calloc(PATH_MAX, 1)))
		return (-1);
	getcwd(workdir, PATH_MAX);
	ft_putendl_fd(workdir, obj->redir->cmd_output);
	free(workdir);
	return (0);
}

int		parse_export(t_obj *obj, char *input, int *i, t_env *env)
{
	(void)env;
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in export\n");
	return (0);
}

int		parse_unset(t_obj *obj, char *input, int *i, t_env *env)
{
	char	**elt;
	char	*tmp;
	char	*sample;

	tmp = ft_strdup("");
	while (is_end(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		if (find_redir(obj->redir, input, i) < 0 || \
			!(sample = sample_str(input, i, sample)))
			return (free_two_str(sample, tmp));
		if (ft_strchr(sample, '=') || ft_strspchr(sample))
		{
			dprintf(obj->redir->err_output, \
			"unset: %s: invalid parameter name\n", sample);
			return (free_two_str(sample, tmp));
		}
		tmp = ft_strjoin_sp(tmp, sample);
	}
	if (!(elt = ft_split(tmp, ' ')))
		return (-1);
	free(tmp);
	unset_var(elt, env);
	free_array(elt, -1);
	return (0);
}

int		parse_env(t_obj *obj, char *input, int *i, t_env *env)
{
	while (input[*i] && is_separator(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		if (find_redir(obj->redir, input, i) == 0)
		{
			while (*i < (int)ft_strlen(input))
				if (find_redir(obj->redir, input, i) == 0)
					(*i)++;
			ft_putstr_fd("env: too many arguments\n", obj->redir->err_output);
			return (-1);
		}
	}
	print_env(env, obj->redir->cmd_output);
	return (0);
}

int		parse_exit(t_obj *obj, char *input, int *i, t_env *env)
{
	(void)env;
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in exit\n");
	return (0);
}
