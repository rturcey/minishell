/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/04/29 12:26:48 by rturcey          ###   ########.fr       */
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
	ft_printf("%s%s@%s%s:%s%s%s$ ", GREEN, user, host, END, CYAN, path, END);
	free(host);
	free(path);
	close(fd);
}

int			main(int argc, char **argv, char **env)
{
	char	*line;
	t_env	*lstenv;

	(void)argc;
	(void)argv;
	if (!(lstenv = init_env(env)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return(-1);
	}
	while (1)
	{
		prompt(lstenv);
		get_next_line(0, &line);
		//test print env + test leaks
		if (ft_strncmp(line, "env", 4) == 0)
			print_env(lstenv);
		else if (ft_strncmp(line, "exit", 5) == 0)
		{
			free(line);
			break;
		}
		else
			general_parser(line);
		free(line);
	}
	env_clear(lstenv);
	return (0);
}
