/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/04 11:19:06 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void		sighandler(int num)
{
	if (num == SIGINT)
	{
		if (g_forked == 0)
		{
			ft_dprintf(2, "\n");
			prompt(g_lstenv);
		}
		else if (g_forked == 1)
			ft_dprintf(2, "\n");
		g_err = 130;
	}
	else if (num == SIGQUIT)
	{
		if (g_forked == 1)
		{
			ft_dprintf(2, "Quit: (core dumped)\n");
			g_err = 131;
		}
		else if (g_forked == 0)
			ft_dprintf(2, "\b\b \b  \b\b");
	}
}

static void	routine(t_sh *sh, char **line)
{
	sh->pip->count = 0;
	sh->pip->lever = 0;
	sh->pip->type = 0;
	prompt(g_lstenv);
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	if (get_next_line(0, line) == 0 && g_forked != 3)
	{
		if (*line)
			free(*line);
		*line = ft_strdup("exit\n");
	}
	sh->in = *line;
}

int			main(int argc, char **argv, char **env)
{
	char		*line;
	int			ret;
	t_sh		*sh;

	(void)argc;
	(void)argv;
	g_lstenv = NULL;
	g_forked = 0;
	g_err = 0;
	if (init_main(&sh, env) == -1)
		return (-1);
	while (1)
	{
		routine(sh, &line);
		ret = general_parser(sh);
		free(line);
		if (ret != 0)
			break ;
	}
	ret = g_err;
	clear_sh(sh);
	return (ret);
}
