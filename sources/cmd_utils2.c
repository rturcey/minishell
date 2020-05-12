/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:29:07 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/12 06:57:30 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**DUMMY PARSING FUNCTIONS
*/

int		parse_pwd(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*workdir;
	int		r;

	workdir = NULL;
	(void)env;
	while (input[*i] && is_separator(input, *i) == 0)
	{
		if ((r = find_redir(obj, input, i) == 0))
		{
			while (*i < (int)ft_strlen(input))
			{
				if ((r = find_redir(obj, input, i) == 0))
					(*i)++;
				else if (r == -1)
					return (-1);
			}
		}
		else if (r == -1)
			return (-1);
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

	while (is_end(input, *i) == 0)
	{
		if (redir_loop(obj, input, i) == -1)
			return (-1);
		if (!(sample = sample_str(input, i, sample, env)))
			return (free_str(sample));
		if (sample_export(sample, env) == -1)
			maj_err(obj, ft_sprintf(\
			"export: %s : not a valid identifier", sample), 1);
		free(sample);
	}
	return (print_result(obj, 0, NULL));
}

int		parse_unset(t_obj *obj, char *input, int *i, t_env *env)
{
	char	**elt;
	char	*tmp;
	char	*sample;

	tmp = ft_strdup("");
	while (is_end(input, *i) == 0)
	{
		if (redir_loop(obj, input, i) == -1)
			return (free_str(tmp));
		if (!(sample = sample_str(input, i, sample, env)))
			return (free_two_str(sample, tmp));
		if (check_var(sample) != 0)
			maj_err(obj, ft_sprintf("unset: %s: not a valid identifier\n", \
			sample), 1);
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
	int		r;

	while (is_end(input, *i) == 0)
	{
		if ((r = find_redir(obj, input, i)) == 0)
		{
			while (*i < (int)ft_strlen(input))
			{
				if ((r = find_redir(obj, input, i)) == 0)
					(*i)++;
				else if (r == -1)
					return (-1);
			}
			maj_err(obj, ft_strdup("env: too many arguments\n"), 1);
		}
		else if (r == -1)
			return (-1);
	}
	if (obj->error)
		return (print_result(obj, 0, NULL));
	print_env(env, obj->redir->cmd_output);
	return (0);
}

int		parse_exit(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*s;
	int		r;

	s = NULL;
	while (input[*i])
	{
		if (redir_loop(obj, input, i) == -1)
			return (-1);
		if (is_end(input, *i) == 1)
			break ;
		if (s != NULL && ((ft_numstr(s) != 1) || (g_err = 1)))
			*i = find_string_end(input, *i);
		else if ((s == NULL) && !(s = sample_str(input, i, s, env)))
			return (-1);
	}
	maj_err(obj, ft_sprintf("exit\n"), g_err);
	if (g_err == 1)
		maj_err(obj, ft_sprintf("exit: too many arguments\n"), 1);
	else if ((s != NULL) && ft_numstr(s) == 1 && (r = ft_atoi(s)))
		g_err = (r < 0 || r > 255) ? r % 256 : r;
	else if ((s != NULL) && (ft_numstr(s) != 1) && (g_err = 2))
		maj_err(obj, ft_sprintf("exit: %s: numeric argument required\n", s), 2);
	return (print_result(obj, g_err, s));
}
