/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 09:28:45 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/06 11:52:04 by rturcey          ###   ########.fr       */
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

/*
** export a shell variable into environment
** if it already exists, replace the value
** if value is empty (and key isn't already in env), var isn't exported
** returns -1 in case of error
*/

int				add_var(t_env *elt, t_env *env)
{
	t_env	*tmp;
	t_env	*bis;

	tmp = env;
	if (!elt->key || !elt->value)
		return (-1);
	if (!(bis = find_env_entry(elt->key, env)))
	{
		if (ft_strlen(elt->value) != 0)
		{
			while (tmp->next)
				tmp = tmp->next;
			if (!(tmp->next = env_cpy(elt)))
				return (-1);
		}
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
	if (!(bis = find_env_entry(elt->key, env)) || bis->in == 0)
	{
		if (ft_strlen(elt->value) != 0)
		{
			while (tmp->next)
				tmp = tmp->next;
			if (!(tmp->next = env_cpy(elt)))
				return (-1);
		}
		return (0);
	}
	free(bis->value);
	bis->value = NULL;
	bis->value = ft_strdup(elt->value);
	return (0);
}
