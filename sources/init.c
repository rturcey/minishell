/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 10:06:54 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/05 12:12:02 by rturcey          ###   ########.fr       */
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
	sh->running_nb = 0;
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
	t_env	*shlvl;
	int		lvl;

	if (!(g_lstenv = init_env(env, 1)))
		return (env_clear(g_lstenv));
	if (!(shlvl = find_env_entry("SHLVL", g_lstenv)))
		return (env_clear(g_lstenv));
	else
	{
		lvl = ft_atoi(shlvl->val);
		free(shlvl->val);
		if (!(shlvl->val = ft_itoa(lvl + 1)))
			return (env_clear(g_lstenv));
	}
	if (!(*sh = init_sh(g_lstenv)))
		return (env_clear(g_lstenv));
	return (0);
}
