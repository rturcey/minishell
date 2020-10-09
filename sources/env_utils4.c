/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 10:32:39 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/08 09:52:22 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void			find_home_user(char *path, char **home, char **user, t_env *env)
{
	int		i;
	int		j;
	char	*tmp;

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
	if (!(tmp = ft_strjoin("HOME=", *home)))
		exit(EXIT_FAILURE);
	sample_export(tmp, env, 1);
	find_env_entry("HOME", env)->in = 0;
	free(tmp);
	if (!(tmp = ft_strjoin("USER=", *user)))
		exit(EXIT_FAILURE);
	sample_export(tmp, env, 1);
	find_env_entry("USER", env)->in = 0;
	free(tmp);
}

void			*check_empty(int lever, char **env)
{
	if (lever == 0)
		return (NULL);
	return (char_free_array(env, 6));
}

int				find_path(t_env *lst)
{
	if (!(lst->key = ft_strdup("PATH")))
		return (-1);
	if (!(lst->val = \
	ft_strdup("/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin")))
		return (-1);
	lst->in = 0;
	return (0);
}

char			**empty_env(void)
{
	char	**tab;

	if (!(tab = malloc(7 * sizeof(char *))))
		return (NULL);
	if (!(tab[0] = ft_strdup("LS_COLORS=")))
		return (char_free_array(tab, 6));
	if (!(tab[1] = ft_strdup("LESSCLOSE=/usr/bin/lesspipe %s %s")))
		return (char_free_array(tab, 6));
	if (!(tab[2] = ft_strjoin_bth(ft_strdup("PWD="), get_pwd(NULL))))
		return (char_free_array(tab, 6));
	if (!(tab[3] = ft_strdup("SHLVL=0")))
		return (char_free_array(tab, 6));
	if (!(tab[4] = ft_strdup("LESSOPEN=| /usr/bin/lesspipe %s")))
		return (char_free_array(tab, 6));
	if (!(tab[5] = ft_strdup("_=/usr/bin/env")))
		return (char_free_array(tab, 6));
	tab[6] = 0;
	return (tab);
}
