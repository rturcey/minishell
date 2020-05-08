/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 09:28:45 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/08 09:54:50 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				unset_var(char **elt, t_env *env)
{
	int		i;

	i = -1;
	while (elt[++i])
		del_from_key(&env, elt[i]);
	return (0);
}

int				sample_export(char *sample, t_env *env)
{
	int		i;
	t_env	*new;
	t_env	*tmp;

	i = -1;
	while (sample[++i] && sample[i] != '=')
	{
		if (is_space(sample, i) != 0)
			return (-1);
	}
	if (!sample[i] && (tmp = find_env_entry(sample, env)))
		tmp->in = 1;
	else if (sample[i] == '=')
	{
		if (!(new = env_new(1)))
			return (-2);
		if (split_env(sample, new) == -1)
			return (-1);
		export_var(new, env);
		del_var(new);
	}
	return (0);
}

int				add_var(t_env *elt, t_env *env)
{
	t_env	*tmp;
	t_env	*bis;

	tmp = env;
	if (!elt->key || !elt->value)
		return (-1);
	if (!(bis = find_env_entry(elt->key, env)))
	{
		while (tmp->next)
			tmp = tmp->next;
		if (!(tmp->next = env_cpy(elt)))
			return (-1);
		return (0);
	}
	free(bis->value);
	bis->value = NULL;
	bis->value = ft_strdup(elt->value);
	return (0);
}

int				export_var(t_env *elt, t_env *env)
{
	t_env	*tmp;
	t_env	*bis;

	tmp = env;
	if (!elt->key || !elt->value)
		return (-1);
	if (!(bis = find_env_entry(elt->key, env)))
	{
		while (tmp->next)
			tmp = tmp->next;
		if (!(tmp->next = env_cpy(elt)))
			return (-1);
		tmp->next->in = 1;
		return (0);
	}
	free(bis->value);
	bis->value = NULL;
	bis->value = ft_strdup(elt->value);
	bis->in = 1;
	return (0);
}
