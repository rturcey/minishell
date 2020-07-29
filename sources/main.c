/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/07/29 15:32:06 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_sh	*init_sh(t_env *env)
{
	t_sh	*sh;

	if (!(sh = malloc(sizeof(t_sh))))
		return (NULL);
	if (!(sh->pip = malloc(sizeof(t_pipe))))
	{
		free(sh);
		return (NULL);
	}
	sh->env = env;
	sh->lev = 1;
	sh->err = 0;
	return (sh);
}

static void	clear_sh(t_sh *sh)
{
	if (sh->env)
		env_clear(sh->env);
	free (sh->pip);
	free(sh);
}

static void	remove_home_path(char **path, char *home)
{
	size_t	i;
	size_t	j;
	char	*tmp;

	if (!home)
		return ;
	i = ft_strlen(home);
	if (ft_strncmp(*path, home, i) != 0)
		return ;
	if (!(tmp = malloc(ft_strlen(*path) - i + 2)))
		return ;
	tmp[0] = '~';
	i--;
	j = 0;
	while (++i <= ft_strlen(*path))
		tmp[++j] = (*path)[i];
	tmp[j] = '\0';
	free(*path);
	*path = tmp;
}

static void	prompt(t_env *env)
{
	char	*path;
	char	*user;
	char	*host;
	char	*home;
	int		fd;

	fd = open("/etc/hostname", O_RDONLY);
	get_next_line(fd, &host);
	path = find_env_val("PWD", env);
	user = find_env_val("USER", env);
	home = find_env_val("HOME", env);
	remove_home_path(&path, home);
	ft_dprintf(2, "%s%s@%s%s:%s%s%s %s►%s ", YELLOW, user, host, END, CYAN, path, \
	END, YELLOW, END);
	free_two_str(user, home);
	free_two_str(path, host);
	close(fd);
}

int			main(int argc, char **argv, char **env)
{
	char		*line;
	t_env		*lstenv;
	int			ret;
	t_sh		*sh;

	(void)argc;
	(void)argv;
	if (!(lstenv = init_env(env, 1)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return (0);
	}
	if (!(sh = init_sh(lstenv)))
	{
		env_clear(lstenv);
		return (-1);
	}
	while (1)
	{
		sh->pip->count = 0;
		sh->pip->lever = 0;
		prompt(lstenv);
		get_next_line(0, &line);
		sh->input = line;
		ret = general_parser(sh);
		free(line);
		if (ret != 0)
			break ;
	}
	ret = sh->err;
	clear_sh(sh);
	return (ret);
}
