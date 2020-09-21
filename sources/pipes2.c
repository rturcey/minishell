/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/08 15:17:24 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/10 17:30:49 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void		parent_handling(t_sh *sh)
// {
// 	int	status;

// 	status = 0;
// 	close(sh->pip->pipefd[1]);
// 	if (sh->pip->type == 1)
// 	{
// 		wait(&status);
// 		while (!WIFEXITED(status))
// 			if (!WIFSIGNALED(status))
// 				break ;
// 		// ft_dprintf(2, "IN PARENTHANDLE bef g_err %d\n", g_err);
// 		// if (WIFEXITED(status))
// 		// 	g_err = WEXITSTATUS(status);
// 		// ft_dprintf(2, "IN PARENTHANDLE aft g_err %d\n", g_err);
// 	}
// 	ft_dprintf(2, "IN PARENTHANDLE bef [%s] sh->pip->type %d g_err %d\n", sh->obj->obj, sh->pip->type, g_err);
// 	if (sh->pip->type == 0 && WIFEXITED(status))
// 		g_err = WEXITSTATUS(status);
// 	ft_dprintf(2, "IN PARENTHANDLE aft [%s] g_err %d\n", sh->obj->obj, g_err);
// 	sh->pip->count--;
// 	exit(g_err);
// }


void		parent_handling(t_sh *sh)
{
	int	status;

	status = 0;
	ft_dprintf(2, ">> PARENT_HANDLING [%s], type = [%d]\n", sh->obj->obj, sh->pip->type);
	close(sh->pip->pipefd[1]);
	while (!WIFEXITED(status))
		if (!WIFSIGNALED(status))
			break ;
	if (sh->pip->type == 1)
	{
		ft_dprintf(2, ">> PARENT_HANDLING [%s], before wait, g_err %d\n", sh->obj->obj, g_err);
		wait(&status);
		if (WIFEXITED(status))
			g_err = WEXITSTATUS(status);
	}
	sh->pip->count--;
	ft_dprintf(2, ">> PARENT_HANDLING [%s], type = [%d] BEFORE EXIT %d\n", sh->obj->obj, sh->pip->type, g_err);
	if (sh->obj)
		free_obj(sh->obj);
	exit(g_err);
}