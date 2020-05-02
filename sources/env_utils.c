/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 10:03:43 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/02 18:28:38 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** lstnew for t_env
*/

t_env			*env_new(void)
{
	t_env	*new;

	if (!(new = malloc(sizeof(t_env))))
		return (NULL);
	new->key = NULL;
	new->value = NULL;
	new->next = NULL;
	return (new);
}

/*
** split key=value in 2 str, put them in a t_env elt
*/

static int		split_env(char *line, t_env *elt)
{
	int		i;
	int		j;

	i = 0;
	j = -1;
	while (line[i] && line[i] != '=')
		i++;
	if (!line[i])
		return (-1);
	if (!(elt->key = malloc(i + 1)))
		return (-1);
	while (++j < i)
		elt->key[j] = line[j];
	elt->key[j] = '\0';
	if (!(elt->value = ft_strdup(&line[i + 1])))
		return (-1);
	return (0);
}

void			*env_clear(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
	return (NULL);
}

void			print_env(t_env *env, int fd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		dprintf(fd, "%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

/*
** conversion from char** to t_env*
** & check if input isn't corrupted
*/

t_env			*init_env(char **env)
{
	t_env	*lst;
	t_env	*tmp;
	int		i;

	i = 0;
	if (!env || !env[0] || !(lst = env_new()))
		return (NULL);
	tmp = lst;
	if (split_env(env[0], lst) == -1)
		return (env_clear(tmp));
	while (env[++i])
	{
		if (!(lst->next = env_new()))
			return (env_clear(tmp));
		lst = lst->next;
		if (split_env(env[i], lst) == -1)
			return (env_clear(tmp));
	}
	return (tmp);
}
