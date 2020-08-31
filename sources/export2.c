/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 11:20:04 by rturcey           #+#    #+#             */
/*   Updated: 2020/08/31 13:19:34 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*env_double(t_env *env)
{
	t_env	*new;

	if (!(new = malloc(sizeof(t_env))))
		return (NULL);
	new->key = ft_strdup(env->key);
	new->val = ft_strdup(env->val);
	new->in = 1;
	new->next = NULL;
	new->pluseq = 0;
	return (new);
}

static void		env_sort(t_env **env)
{
	t_env	*save;
	t_env	*elt;
	char	*tmp1;
	char	*tmp2;

	save = *env;
	while (save)
	{
		elt = *env;
		while (elt->next)
		{
			if (ft_strcmp(elt->key, elt->next->key) > 0)
			{
				tmp1 = elt->key;
				tmp2 = elt->val;
				elt->key = elt->next->key;
				elt->val = elt->next->val;
				elt->next->key = tmp1;
				elt->next->val = tmp2;
			}
			elt = elt->next;
		}
		save = save->next;
	}
}

static void		free_export(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->val);
		free(env->key);
		free(env);
		env = tmp;
	}
}

static void		print_export(int fd, t_env *env)
{
	t_env	*save;

	save = env;
	while (env)
	{
		if (env->val[0] && env->key[0] != '_')
			dprintf(fd, "declare -x %s=\"%s\"\n", env->key, env->val);
		else if (env->key[0] != '_')
			dprintf(fd, "declare -x %s\n", env->key);
		env = env->next;
	}
	free_export(save);
}

void			export_solo(t_sh *sh)
{
	t_env	*ptr;
	t_env	*begin;
	t_env	*save;

	begin = sh->env;
	while (begin && begin->in != 1)
		begin = begin->next;
	if (!(ptr = env_double(begin)))
		return ;
	save = ptr;
	begin = begin->next;
	while (begin)
	{
		if (begin->in == 1 && !(ptr->next = env_double(begin)))
		{
			free_export(save);
			return ;
		}
		ptr = ptr->next;
		begin = begin->next;
	}
	env_sort(&save);
	print_export(sh->obj->redir->cmd_output, save);
}
