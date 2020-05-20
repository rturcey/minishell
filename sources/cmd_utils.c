/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:56:26 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/20 12:07:14 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Cmd array init and strncmp with sample from input
**return appropriate index (corresponding to cmd function)
*/

int		is_cmd(char *sample)
{
	char	*cmds[8];
	int		j;

	cmds[0] = "echo";
	cmds[1] = "cd";
	cmds[2] = "pwd";
	cmds[3] = "export";
	cmds[4] = "unset";
	cmds[5] = "env";
	cmds[6] = "exit";
	cmds[7] = NULL;
	j = -1;
	while (cmds[++j] != NULL)
	{
		if ((ft_strlen(cmds[j]) == ft_strlen(sample)) &&
			ft_strncmp(cmds[j], sample, ft_strlen(sample)) == 0)
			return (j);
	}
	return (-1);
}

/*
**DUMMY PARSING FUNCTIONS
*/

int		parse_echo(t_obj *obj, char *input, int *i, t_env *env)
{
	char	*result;
	char	*sample;
	int		l;

	(void)env;
	if (!(result = ft_strdup("")))
		return (-1);
	if (redir_loop(obj, input, i) == -1)
		return (free_str(result));
	pass_option(obj, input, i);
	while (is_end(input, *i) == 0)
	{
		l = 0;
		if (redir_loop(obj, input, i) == -1)
			return (free_str(result));
		if (is_end(input, *i) == 1)
			break ;
		if (ft_strncmp(&input[*i], "\"\"", 2) == 0)
			l = 1;
		if (!(sample = sample_str(input, i, sample, env)))
			return (free_two_str(result, sample));
		if (l == 0 && sample[0] == '\0')
			free(sample);
		else
			result = ft_strjoin_sp(result, sample);
	}
	if (obj->option != 1)
		result = ft_strjoin_bth(result, ft_strdup("\n"));
	obj->result = result;
	return (print_result(obj, 0, NULL));
}

int		replace_pwd(t_env *env, char **path)
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

int		parse_cd(t_obj *obj, char *input, int *i, t_env *env)
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
	if (ret > 0)
		maj_err(obj, ft_strdup("cd: too many arguments\n"), 1);
	else if (chdir(path) == -1)
		maj_err(obj, ft_sprintf("cd: %s: %s\n", path, strerror(errno)), 1);
	else if ((ret = replace_pwd(env, &path)) == -1)
		return (free_str(path));
	else if (ret == -2)
	{
		maj_err(obj, ft_sprintf("cd: error retrieving current directory:\
			getcwd: cannot access parent directories: No such file or\
			directory\n"), 0);
		chdir(path);
	}
	return (print_result(obj, 0, path));
}

/*
**Cmds parsing functions array, and sends us to
**function corresponding to the matching index
**we got from strncmp
*/

int		parse_cmds(t_obj *obj, char *input, int *i, t_env *env)
{
	t_parse_cmd parse_cmd[7];
	int			ret;

	parse_cmd[0] = parse_echo;
	parse_cmd[1] = parse_cd;
	parse_cmd[2] = parse_pwd;
	parse_cmd[3] = parse_export;
	parse_cmd[4] = parse_unset;
	parse_cmd[5] = parse_env;
	parse_cmd[6] = parse_exit;
	if ((ret = parse_cmd[obj->type](obj, input, i, env)) == -1)
	{
		while (is_end(input, *i) == 0)
			(*i)++;
		return (-1);
	}
	return (ret);
}
