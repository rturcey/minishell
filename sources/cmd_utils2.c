/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:29:07 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/08 09:32:21 by rturcey          ###   ########.fr       */
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
		if (find_redir(obj->redir, input, i) == 0)
		{
			while (*i < (int)ft_strlen(input))
				if (find_redir(obj->redir, input, i) == 0)
					(*i)++;
		}
	}
	if (!(workdir = ft_calloc(PATH_MAX, 1)))
		return (-1);
	getcwd(workdir, PATH_MAX);
	obj->result = ft_strjoin_bth(workdir, ft_strdup("\n"));
	return (print_result(obj, 0, NULL));
}

int		parse_export(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*sample;
	int		r;

	while (is_end(input, *i) == 0)
	{
		while ((r = find_redir(obj->redir, input, i)) == 1)
			r++;
		if (!(sample = sample_str(input, i, sample)))
			return (free_str(sample));
		if (sample_export(sample, env) == -1)
			maj_err(obj, ft_sprintf("bash: export: « %s » : identifiant non valable", sample));
		free(sample);
	}
	return (print_result(obj, 0, NULL));
}

int		parse_unset(t_obj *obj, char *input, int *i, t_env *env)
{
	char	**elt;
	char	*tmp;
	char	*sample;
	int		r;

	tmp = ft_strdup("");
	while (is_end(input, *i) == 0)
	{
		while ((r = find_redir(obj->redir, input, i)) == 1)
			r++;
		if (!(sample = sample_str(input, i, sample)))
			return (free_two_str(sample, tmp));
		if (ft_strchr(sample, '=') || ft_strspchr(sample))
			maj_err(obj, ft_sprintf("unset: %s: invalid parameter name\n", \
			sample));
		tmp = ft_strjoin_sp(tmp, sample);
	}
	if (!(elt = ft_split(tmp, ' ')))
		return (-1);
	free(tmp);
	unset_var(elt, env);
	free_array(elt, -1);
	return (print_result(obj, 0, NULL));
}

int		parse_env(t_obj *obj, char *input, int *i, t_env *env)
{
	while (is_end(input, *i) == 0)
	{
		if (find_redir(obj->redir, input, i) == 0)
		{
			while (*i < (int)ft_strlen(input))
				if (find_redir(obj->redir, input, i) == 0)
					(*i)++;
			maj_err(obj, ft_strdup("env: too many arguments\n"));
		}
	}
	if (obj->error)
		return (print_result(obj, 0, NULL));
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
