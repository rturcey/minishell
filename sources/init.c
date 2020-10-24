/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 12:18:34 by user42            #+#    #+#             */
/*   Updated: 2020/10/24 19:13:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_sh	*init_sh(t_env *env)
{
	t_sh	*sh;

	if (!(sh = malloc(sizeof(t_sh))))
		return (NULL);
	sh->in = NULL;
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
	if (sh->in)
		free(sh->in);
	while (sh->obj)
		free_obj(&sh->obj);
	free(sh);
}

int			init_main(t_sh **sh, char **env)
{
	t_env	*shlvl;
	int		lvl;

	g_sigint = 0;
	if (!(g_lstenv = init_env(env, 1, 0)))
		return (env_clear(g_lstenv));
	if (!(shlvl = find_env_entry("SHLVL", g_lstenv)))
		return (env_clear(g_lstenv));
	else
	{
		lvl = ft_atoi(shlvl->val);
		free_two_str(shlvl->val, shlvl->val_sp);
		if (!(shlvl->val = ft_itoa(lvl + 1)) ||
		 !(shlvl->val_sp = ft_strdup(shlvl->val)))
			return (env_clear(g_lstenv));
	}
	if (!(*sh = init_sh(g_lstenv)))
		return (env_clear(g_lstenv));
	g_pid = -1;
	return (0);
}
