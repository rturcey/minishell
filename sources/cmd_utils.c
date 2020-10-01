/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:56:26 by esoulard          #+#    #+#             */
/*   Updated: 2020/10/01 11:34:05 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**DUMMY PARSING FUNCTIONS
*/

static int	echo_loop(char **result, int *i, t_sh *sh)
{
	int		l;
	char	*sample;

	l = 0;
	sample = NULL;
	if (ft_strncmp(&sh->in[*i], "\"\"", 2) == 0)
		l = 1;
	if (!(sample = sample_str(sh, i, sample)))
		return (free_str(*result));
	if (l == 0 && sample[0] == '\0')
		free_str(sample);
	else if (!(*result = ft_strjoin_sp(*result, sample)))
		return (-1);
	return (0);
}

int			parse_echo(t_sh *sh, int *i)
{
	char	*result;

	if (redir_loop(sh, i) == -1)
		return (-1);
	pass_option(sh, i);
	if (!(result = ft_strdup("")))
		return (-1);
	while (is_end(sh->in, *i) == 0)
	{
		if (redir_loop(sh, i) == -1)
			return (free_str(result));
		if (is_end(sh->in, *i) == 1)
			break ;
		if (echo_loop(&result, i, sh) == -1)
			return (-1);
	}
	if (sh->obj->option != 1)
		result = ft_strjoin_bth(result, ft_strdup("\n"));
	sh->obj->result = result;
	return (print_result(sh, 0, NULL));
}

static void	err_cd(t_sh *sh, int ret, char *path)
{
	char	*mg;
	char	*mgb;
	char	*workdir;
	int		i;

	mg = ft_strdup("cd: error retrieving current directory: getcwd: cannot ");
	mgb = ft_strdup("access parent directories: No such file or directory\n");
	if ((i = 0) == 0 && ret > 0)
		maj_err(sh, ft_strdup("cd: too many arguments\n"), 1);
	else if (chdir(path) == -1)
		maj_err(sh, ft_sprintf("cd: %s: %s\n", path, strerror(errno)), 1);
	else if (ret == 0)
	{
		if (!(workdir = ft_calloc(PATH_MAX, 1)))
			return ;
		if (getcwd(workdir, PATH_MAX) == NULL && (i = 1) == 1)
		{
			if (!(mg = ft_strjoin_bth(mg, mgb)))
				return ;
			maj_err(sh, mg, 0);
		}
		free_str(workdir);
	}
	if (i == 0)
		free_two_str(mg, mgb);
}

int			parse_cd(t_sh *sh, int *i)
{
	char	*path;
	int		ret;

	path = NULL;
	ret = 0;
	while (sh->in[*i])
	{
		if (redir_loop(sh, i) == -1)
			return (-1);
		if (is_end(sh->in, *i) == 1)
			break ;
		if ((path != NULL) && ret++)
			*i = find_string_end(sh->in, *i);
		else if ((path == NULL) && !(path = sample_str(sh, i, path)))
			return (-1);
	}
	if ((path == NULL) && !(path = find_env_val("HOME", sh->env)))
		return (-1);
	err_cd(sh, ret, path);
	if ((ret = replace_pwd(sh->env, &path)) == -1)
		return (free_str(path));
	return (print_result(sh, 0, path));
}
