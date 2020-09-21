/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 10:06:54 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/21 20:11:56 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_sh	*init_sh(t_env *env)
{
	t_sh	*sh;

	if (!(sh = malloc(sizeof(t_sh))))
		return (NULL);
	sh->env = env;
	sh->lev = 1;
	sh->obj = NULL;
	sh->running = NULL;
	sh->wait = 0;
	return (sh);
}

void		clear_sh(t_sh *sh)
{
	if (sh->env)
		env_clear(sh->env);
	while (sh->obj)
		free_obj(&sh->obj);
	free(sh);
}

int			init_main(t_sh **sh, char **env)
{
	if (!(g_lstenv = init_env(env, 1)))
	{
		ft_putstr_fd("couldn't clone the environment", 2);
		return (-1);
	}
	if (!(*sh = init_sh(g_lstenv)))
	{
		env_clear(g_lstenv);
		return (-1);
	}
	return (0);
}
