/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:56:26 by esoulard          #+#    #+#             */
/*   Updated: 2020/07/27 09:44:45 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**DUMMY PARSING FUNCTIONS
*/

static int	echo_loop(char **result, char *input, int *i, t_env *env)
{
	int		l;
	char	*sample;

	l = 0;
	sample = NULL;
	if (ft_strncmp(&input[*i], "\"\"", 2) == 0)
		l = 1;
	if (!(sample = sample_str(input, i, sample, env)))
		return (free_str(*result));
	if (l == 0 && sample[0] == '\0')
		free(sample);
	else if (!(*result = ft_strjoin_sp(*result, sample)))
		return (-1);
	return (0);
}

int			parse_echo(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*result;

	(void)env;
	if (redir_loop(obj, input, i) == -1)
		return (-1);
	pass_option(obj, input, i);
	if (!(result = ft_strdup("")))
		return (-1);
	while (is_end(input, *i) == 0)
	{
		if (redir_loop(obj, input, i) == -1)
			return (free_str(result));
		if (is_end(input, *i) == 1)
			break ;
		if (echo_loop(&result, input, i, env) == -1)
			return (-1);
	}
	if (obj->option != 1)
		result = ft_strjoin_bth(result, ft_strdup("\n"));
	obj->result = result;
	return (print_result(obj, 0, NULL));
}

int			replace_pwd(t_env *env, char **path)
{
	t_env	*pwd;
	char	*workdir;
	int		ret;

	pwd = env_new(1);
	if (!(workdir = ft_calloc(PATH_MAX, 1)))
		return (-1);
	ret = 0;
	if ((getcwd(workdir, PATH_MAX) == NULL) && (ft_strncmp(*path, ".",
		ft_strlen(*path)) == 0) && (ret = -2))
	{
		if (!(workdir = ft_strjoin_slash(find_env_val("PWD", env), ".")))
			return (-1);
		free_str(*path);
		*path = ft_strdup(workdir);
	}
	else if (getcwd(workdir, PATH_MAX) == NULL)
		return (free_str(workdir));
	if (!(pwd->val = workdir))
		return (-1);
	if (!(pwd->key = ft_strdup("PWD")))
		return (free_str(workdir));
	add_var(pwd, env);
	del_var(pwd);
	return (ret);
}

static void	err_cd(t_obj *obj, int ret, char *path)
{
	char	*mg;
	char	*mgb;

	mg = ft_strdup("cd: error retrieving current directory: getcwd: cannot ");
	mgb = ft_strdup("access parent directories: No such file or directory\n");
	if (ret > 0)
		maj_err(obj, ft_strdup("cd: too many arguments\n"), 1);
	else if (chdir(path) == -1)
		maj_err(obj, ft_sprintf("cd: %s: %s\n", path, strerror(errno)), 1);
	else if (ret == -2)
	{
		if (!(mg = ft_strjoin_bth(mg, mgb)))
			return ;
		maj_err(obj, mg, 0);
		chdir(path);
	}
}

int			parse_cd(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*path;
	int		ret;

	path = NULL;
	ret = 0;
	while (input[*i])
	{
		if (redir_loop(obj, input, i) == -1)
			return (-1);
		if (is_end(input, *i) == 1)
			break ;
		if ((path != NULL) && ret++)
			*i = find_string_end(input, *i);
		else if ((path == NULL) && !(path = sample_str(input, i, path, env)))
			return (-1);
	}
	if ((path == NULL) && !(path = ft_strdup(find_env_val("HOME", env))))
		return (-1);
	err_cd(obj, ret, path);
	if ((ret = replace_pwd(env, &path)) == -1)
		return (free_str(path));
	return (print_result(obj, 0, path));
}
