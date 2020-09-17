/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:23:39 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/17 16:48:55 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** lstnew for redirection
*/

t_redir		*redir_new(void)
{
	t_redir	*new;

	if (!(new = malloc(sizeof(t_redir))))
		return (NULL);
	new->cmd_in = 0;
	new->cmd_output = 1;
	new->err_output = 2;
	return (new);
}

/*
** lstnew for object
**(same as plotted on framapad)
*/

t_obj		*obj_new(t_env *env, t_obj *prev)
{
	t_obj	*new;

	if (!(new = malloc(sizeof(t_obj))))
		return (NULL);
	new->obj = NULL;
	new->type = 0;
	new->option = 0;
	new->redir = NULL;
	new->result = NULL;
	new->env = env;
	new->pip = 0;
	new->args = NULL;
	new->charenv = NULL;
	new->error = 0;
	new->next = NULL;
	new->prev = prev;
	return (new);
}

/*
**initialize an object, need to add potential redir (see below)
*/

char		*init_obj(t_obj *obj, char *sample, int type)
{
	if (!(obj->obj = ft_strdup(sample)))
		return (NULL);
	obj->type = type;
	return (obj->obj);
}

/*
**TO DO
**if stock_redir != NULL
**send to a function that parses stock redir and
**puts it in the initialized obj
**and free stock_redir
*/

/*void		parent_handling(t_sh *sh)
{
	int	status;

	status = 0;
	//ft_dprintf(2, "name = %s, type = %d\n", sh->obj->obj, sh->pip->type);
	close(sh->pip->pipefd[1]);
	while (!WIFEXITED(status))
		if (!WIFSIGNALED(status))
			break ;
	if (sh->pip->type == 1)
		wait(&status);
	if (WIFEXITED(status))
		g_err = WEXITSTATUS(status);
	sh->pip->count--;
	exit(g_err);
}*/
