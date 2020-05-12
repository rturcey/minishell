/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/12 22:02:28 by esoulard         ###   ########.fr       */
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

	if (!(path = malloc(PATH_MAX)))
		return ;
	fd = open("/etc/hostname", O_RDONLY);
	get_next_line(fd, &host);
	getcwd(path, PATH_MAX);
	user = find_env_value("USER", env);
	home = find_env_value("HOME", env);
	remove_home_path(&path, home);
	ft_printf("%s%s@%s%s:%s%s%s %s►%s ", YELLOW, user, host, END, CYAN, path, \
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
	t_both_env	*both_env;

	(void)argc;
	(void)argv;
	if (!(lstenv = init_env(env, 1)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return (0);
	}
	if (!(both_env = group_both_env(lstenv, env)))
	{
		env_clear(lstenv);
		return (-1);
	}
	if (!(test = env_new(0)))
	{
		env_clear(lstenv);
		return (-1);
	}
	test->key = ft_strdup("TEST_VAR");
	test->value = ft_strdup("");
	g_err = 0;
	while (1)
	{
		prompt(lstenv);
		get_next_line(0, &line);
		//test print env + test exit + test export
		ret = general_parser(line, lstenv, both_env);
		free(line);
		if (ret != 0)
			break ;
	}
	env_clear(lstenv);
	free(both_env);
	env_clear(test);
	return (g_err);
}
