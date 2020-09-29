/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 10:32:39 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/29 11:19:15 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		find_home_user(char *path, char **home, char **user)
{
	int		i;
	int		j;

	i = 1;
	while (path[i] != '/')
		++i;
	j = i + 1;
	while (path[j] != '/')
		++j;
	if (!(*home = ft_substr(path, 0, j)))
		exit(EXIT_FAILURE);
	if (!(*user = ft_substr(path, i + 1, j - i - 1)))
		exit(EXIT_FAILURE);
}

void		*check_empty(int lever, char **env)
{
	if (lever == 0)
		return (NULL);
	return (char_free_array(env, 6));
}

char		*var_path(void)
{
	char	*line;
	int		fd;
	int		ret;

	if ((fd = open("/etc/environment", O_RDONLY)) == -1)
		return (NULL);
	while ((ret = get_next_line(fd, &line)) >= 0)
	{
		if (ft_strstr(line, "PATH="))
		{
			close(fd);
			return (line);
		}
		else
			free(line);
		if (ret == 0)
			break ;
	}
	close(fd);
	return (NULL);
}

char		**empty_env(void)
{
	char	**tab;

	if (!(tab = malloc(7 * sizeof(char *))))
		return (NULL);
	if (!(tab[0] = ft_strdup("LS_COLORS=")))
		return (char_free_array(tab, 6));
	if (!(tab[1] = ft_strdup("LESSCLOSE=/usr/bin/lesspipe %s %s")))
		return (char_free_array(tab, 6));
	if (!(tab[2] = ft_strdup("PWD=")))
		return (char_free_array(tab, 6));
	if (!(tab[3] = ft_strdup("SHLVL=1")))
		return (char_free_array(tab, 6));
	if (!(tab[4] = ft_strdup("LESSOPEN=| /usr/bin/lesspipe %s")))
		return (char_free_array(tab, 6));
	if (!(tab[5] = ft_strdup("_=/usr/bin/env")))
		return (char_free_array(tab, 6));
	tab[6] = 0;
	return (tab);
}
