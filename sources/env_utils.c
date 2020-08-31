/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 10:03:43 by rturcey           #+#    #+#             */
/*   Updated: 2020/08/31 13:15:18 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** lstnew for t_env
*/

t_env	*env_new(int in)
{
	t_env	*new;

	if (!(new = malloc(sizeof(t_env))))
		return (NULL);
	new->key = NULL;
	new->val = NULL;
	new->in = in;
	new->next = NULL;
	new->pluseq = 0;
	return (new);
}

/*
** split key=val in 2 str, put them in a t_env elt
*/

int		split_env(char *line, t_env *elt)
{
	int		i;
	int		j;

	i = 0;
	j = -1;
	while (line[i] && line[i] != '=' && line[i] != '+')
		i++;
	if (line[i] && line[i] == '+' && pluseq(line, i) == 0)
		return (-1);
	if (!(elt->key = malloc(i + 1)))
		return (-1);
	while (++j < i)
		elt->key[j] = line[j];
	elt->key[j] = '\0';
	if (pluseq(line, i) == 1 && (elt->pluseq = 1))
		i++;
	if (!line[i])
		i--;
	if (!(elt->val = ft_strdup(&line[i + 1])))
		return (-1);
	return (0);
}

int		env_clear(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->val);
		free(env);
		env = tmp;
	}
	return (0);
}

void	print_env(t_env *env, int fd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (tmp->in == 1)
			dprintf(fd, "%s=%s\n", tmp->key, tmp->val);
		tmp = tmp->next;
	}
}

/*
** conversion from char** to t_env*
** & check if in isn't corrupted
*/

t_env	*init_env(char **env, int in)
{
	t_env	*lst;
	t_env	*tmp;
	int		i;

	i = 0;
	if (!env || !env[0] || !(lst = env_new(in)))
		return (NULL);
	tmp = lst;
	if (split_env(env[0], lst) == -1 && env_clear(tmp) == 0)
		return (NULL);
	while (env[++i])
	{
		if (!(lst->next = env_new(in)) && env_clear(tmp) == 0)
			return (NULL);
		lst = lst->next;
		if (split_env(env[i], lst) == -1 && env_clear(tmp) == 0)
			return (NULL);
	}
	return (tmp);
}
