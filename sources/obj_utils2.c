/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:35:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/05 17:42:33 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Free obj structure and sub-structures
**!!! sometimes segfault when freeing arg
*/

void	*free_arg(t_arg *arg)
{
	t_arg	*tmp;

	while (arg)
	{
		tmp = arg->next;
		free(arg->value);
		free(arg);
		arg = tmp;
	}
	return (NULL);
}

void	*free_redir(t_redir *redir)
{
	if (redir->cmd_input != 0)
		close(redir->cmd_input);
	if (redir->cmd_output != 1)
		close(redir->cmd_output);
	if (redir->err_output != 2)
		close(redir->err_output);
	free(redir);
	return (NULL);
}

void	*free_obj(t_obj *obj)
{
	t_obj	*tmp;

	while (obj)
	{
		tmp = obj->next;
		free(obj->obj);
		free_arg(obj->args);
		free_redir(obj->redir);
		free(obj->result);
		free(obj);
		obj = tmp;
	}
	return (NULL);
}
