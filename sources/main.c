/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/04/29 16:33:27 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	ft_printf("%s%s@%s%s:%s%s%s$ ", GREEN, user, host, END, BLUE, path, END);
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
	if (!(lstenv = init_env(env)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return(-1);
	}
	test = env_new();
	test->key = ft_strdup("TEST_VAR");
	test->value = ft_strdup("");
	while (1)
	{
		prompt(lstenv);
		get_next_line(0, &line);
		//test print env + test exit + test export

		if (ft_strncmp(line, "env", 4) == 0)
			print_env(lstenv);
		else if (ft_strncmp(line, "exit", 5) == 0)
		{
			free(line);
			break;
		}
		else if (ft_strncmp(line, "export TEST=", 12) == 0)
		{
			free(test->value);
			test->value = ft_strdup(&line[12]);
			export_var(test, lstenv);
		}
		else
			general_parser(line);
		free(line);
	}
	env_clear(lstenv);
	env_clear(test);
	return (0);
}
