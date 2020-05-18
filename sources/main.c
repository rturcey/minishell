/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/18 20:07:38 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	t_env		*test;
	int			ret;

	(void)argc;
	(void)argv;
	if (!(lstenv = init_env(env, 1)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return (0);
	}
	if (!(test = env_new(0)))
	{
		env_clear(lstenv);
		return (-1);
	}
	test->key = ft_strdup("TEST_VAR");
	test->val = ft_strdup("");
	g_err = 0;
	while (1)
	{
		prompt(lstenv);
		get_next_line(0, &line);
		//test print env + test exit + test export
		ret = general_parser(line, lstenv);
		free(line);
		if (ret != 0)
			break ;
	}
	env_clear(lstenv);
	env_clear(test);
	return (g_err);
}
