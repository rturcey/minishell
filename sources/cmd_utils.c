/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:56:26 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/08 11:12:41 by rturcey          ###   ########.fr       */
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
	int		r;

	(void)env;
	result = ft_strdup("");
	while ((r = find_redir(obj, input, i)) == 1 || r == -1)
		if (r == -1)
			return (-1);
	if (ft_strncmp("-n ", &input[*i], 3) == 0 && ((*i) += 3))
		obj->option = 1;
	while (is_end(input, *i) == 0)
	{
		while ((r = find_redir(obj, input, i)) == 1 || r == -1)
			if (r == -1)
				return (-1);
		if (!(sample = sample_str(input, i, sample)))
			return (free_two_str(result, sample));
		result = ft_strjoin_sp(result, sample);
	}
	if (obj->option != 1)
		result = ft_strjoin_bth(result, ft_strdup("\n"));
	obj->result = result;
	return (print_result(obj, 0, NULL));
}

int		parse_cd(t_obj *obj, char *input, int *i, t_env *env)
{
	char *path;

	path = NULL;
	if ((*i = pass_spaces(input, *i)) && !input[*i])
	{
		if (!(path = ft_strdup(find_env_value("HOME", env))))
			return (-1);
	}
	else if (!(path = sample_str(input, i, path)))
		return (-1);
	else if ((*i = pass_spaces(input, *i)) && (is_end(input, *i) != 1))
	{
		maj_err(obj, ft_strdup("cd: too many arguments\n"));
		return (print_result(obj, 0, path));
	}
	if (chdir(path) == -1)
	{
		maj_err(obj, ft_sprintf("cd: %s: No such file or directory\n", path));
		return (print_result(obj, 0, path));
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

	parse_cmd[0] = parse_echo;
	parse_cmd[1] = parse_cd;
	parse_cmd[2] = parse_pwd;
	parse_cmd[3] = parse_export;
	parse_cmd[4] = parse_unset;
	parse_cmd[5] = parse_env;
	parse_cmd[6] = parse_exit;
	if (parse_cmd[obj->type](obj, input, i, env) == -1)
	{
		while (is_end(input, *i) == 0)
			(*i)++;
		return (-1);
	}
	return (0);
}
