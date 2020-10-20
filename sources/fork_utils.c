/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 12:18:53 by user42            #+#    #+#             */
/*   Updated: 2020/10/20 12:18:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_gfork(t_sh *sh, char *tmp)
{
	if (sh->obj->pip == IS_PIPE || (sh->obj->prev &&
		sh->obj->prev->pip == IS_PIPE))
		g_forked = IS_PIPE;
	else if (tmp && is_ms(tmp) != 0)
		g_forked = IS_EXEC;
	else if (tmp && is_ms(tmp) == 0)
		g_forked = IS_F_MS;
	else
		g_forked = IS_MS;
}

int		is_ms(char *exec)
{
	int i;

	i = ft_strlen(exec) - 1;
	while (exec[i] && exec[i] != '/')
		i--;
	if (i >= 0 && exec[i] == '/')
		i++;
	if (ft_strncmp(&exec[i], "minishell",
		ft_strlen(&exec[i])) == 0)
		return (0);
	return (1);
}

void	kill_pipe(int sig)
{
	int status;
	int ret;

	if (g_forked == IS_PIPE && g_pid != -1)
	{
		while ((ret = wait(&status)) != -1)
		{
			kill(g_pid, sig);
			if (ret == g_pid && sig == SIGINT)
				g_err = 130;
			else if (ret == g_pid && sig == SIGQUIT)
			{
				ft_dprintf(2, "Quit: Core dumped\n");
				g_err = 131;
			}
		}
	}
	g_pid = -1;
}
