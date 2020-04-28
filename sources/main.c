/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:46 by rturcey           #+#    #+#             */
/*   Updated: 2020/04/28 11:56:23 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//importer libft

//va chercher valeur d'une variable d'env (bien penser a ajouter =)
char	*find_value(char **env, const char *str)
{
	int		i;
	int		j;
	size_t	len;

	i = -1;
	len = strlen(str);
	while(env[++i])
	{
		j = 0;
		while(env[i][j] == str[j])
		{
			if (j == (int)len - 1)
				return(&env[i][len]);
			j++;
		}
	}
	return (NULL);
}

int		main(int argc, char **argv, char **env)
{
	char	*buff[101];
	char	*path;

	(void)argc;
	(void)argv;
	(void)env;
	path = malloc(100);
	//trouve le chemin courant pour le prompt:
	getcwd(path, PATH_MAX);
	write(1, path, ft_strlen(path));
	write(1, " $> ", 4);
	free(path);
	//boucler gnl (read juste pour le test):
	read(0, buff, 100);
	return (0);
}

