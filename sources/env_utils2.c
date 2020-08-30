/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:02:11 by rturcey           #+#    #+#             */
/*   Updated: 2020/08/30 12:06:42 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** returns the val associated to key, or NULL if key isn't in env
*/

char			*find_env_val(char *key, t_env *env)
{
	t_env	*tmp;
	int		i;
	int		len;

	tmp = env;
	len = (int)ft_strlen(key) + 1;
	while (tmp && (i = ft_strncmp(key, tmp->key, len)) != 0)
		tmp = tmp->next;
	if (i == 0)
		return (ft_strdup(tmp->val));
	return (ft_strdup(""));
}

/*
** does the same thing but returns the entire elt
*/

t_env			*find_env_entry(char *key, t_env *env)
{
	t_env	*tmp;
	int		i;
	int		len;

	tmp = env;
	len = (int)ft_strlen(key) + 1;
	while (tmp && (i = ft_strncmp(key, tmp->key, len)) != 0)
		tmp = tmp->next;
	if (i == 0)
		return (tmp);
	return (NULL);
}

/*
** returns a copy of a t_env elt
*/

t_env			*env_cpy(t_env *elt)
{
	t_env	*cpy;

	if (!(cpy = env_new(elt->in)))
		return (NULL);
	cpy->key = ft_strdup(elt->key);
	cpy->val = ft_strdup(elt->val);
	return (cpy);
}

/*
** free a t_env var and its data
*/

void			del_var(t_env *var)
{
	free(var->key);
	free(var->val);
	free(var);
	var = NULL;
}

/*
** free a t_env var and its data,
** link previous elt to next elt
*/

void			del_from_key(t_env **begin, char *key)
{
	t_env	*tmp;
	t_env	*env;

	tmp = *begin;
	env = *begin;
	if (ft_strncmp(key, env->key, ft_strlen(env->key)) == 0)
	{
		tmp = (*begin)->next;
		del_var(*begin);
		(*begin) = tmp;
		return ;
	}
	while (env)
	{
		if (ft_strncmp(key, env->key, ft_strlen(env->key)) == 0)
		{
			tmp->next = env->next;
			del_var(env);
			env = tmp;
			return ;
		}
		tmp = env;
		env = env->next;
	}
}
