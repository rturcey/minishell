/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/08/21 16:27:35 by esoulard         ###   ########.fr       */
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
	if (sh->obj)
		free_obj(sh->obj);
	free(sh->pip);
	free(sh);
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
	ft_dprintf(2, "%s%s@%s%s:%s%s%s %s►%s ", YELLOW, user, host, END, \
	CYAN, path, END, YELLOW, END);
	free_two_str(user, home);
	free_two_str(path, host);
	close(fd);
}

int			init_main(t_env **lstenv, t_sh **sh, char **env)
{
	if (!(*lstenv = init_env(env, 1)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return (-1);
	}
	if (!(*sh = init_sh(*lstenv)))
	{
		env_clear(*lstenv);
		return (-1);
	}
	return (0);
}

void		sighandler(int num)
{	
	if (num == SIGINT)
	{
		ft_dprintf(2, "\n");
		prompt(lstenv);
	}
	else if (num == SIGQUIT)
	{
		ft_dprintf(2, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
		//ft_dprintf(2, "sighandler sigquit\n");
	}
}

int			main(int argc, char **argv, char **env)
{
	char		*line;
	// t_env		*lstenv;
	int			ret;
	t_sh		*sh;

	(void)argc;
	(void)argv;
	if (init_main(&lstenv, &sh, env) == -1)
		return (-1);
	while (1)
	{
		sh->pip->count = 0;
		sh->pip->lever = 0;
		sh->pip->type = 0;
		prompt(lstenv);
		signal(SIGINT, sighandler);
		signal(SIGQUIT, sighandler);
		if (get_next_line(0, &line) <= 0)
		{
			if (line)
				free(line);
			line = ft_strdup("exit\n");
		}		
		sh->in = line;
		ret = general_parser(sh);
		free(line);
		if (ret != 0)
			break ;
		//ft_dprintf(2, "end of main loop\n");	
	}
	ret = sh->err;
	clear_sh(sh);
	return (ret);
}
