/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/08 09:50:33 by rturcey          ###   ########.fr       */
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
	int		fd;

	if (!(path = malloc(PATH_MAX)))
		return ;
	fd = open("/etc/hostname", O_RDONLY);
	get_next_line(fd, &host);
	getcwd(path, PATH_MAX);
	user = find_env_value("USER", env);
	remove_home_path(&path, find_env_value("HOME", env));
	ft_printf("%s%s@%s%s:%s%s%s %s►%s ", YELLOW, user, host, END, CYAN, path, \
	END, YELLOW, END);
	free(host);
	free(path);
	close(fd);
}

int			main(int argc, char **argv, char **env)
{
	char	*line;
	t_env	*lstenv;
	t_env	*test;

	(void)argc;
	(void)argv;
	if (!(lstenv = init_env(env, 1)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return (-1);
	}
	test = env_new(0);
	test->key = ft_strdup("TEST_VAR");
	test->value = ft_strdup("");
	while (1)
	{
		prompt(lstenv);
		get_next_line(0, &line);
		//test print env + test exit + test export

		if (ft_strncmp(line, "exit", 5) == 0)
		{
			free(line);
			break ;
		}
		else
			general_parser(line, lstenv);
		free(line);
	}
	env_clear(lstenv);
	env_clear(test);
	return (0);
}
