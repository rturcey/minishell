/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:35:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/01 02:58:36 by esoulard         ###   ########.fr       */
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
		printf("in free arg\n");
		tmp = arg->next;
		free(arg->value);
		printf("after free arg->value\n");
		free(arg);
		printf("after free arg\n");
		arg = tmp;
	}
	return (NULL);
}

void	*free_redir(t_redir *redir)
{
	free(redir->cmd_output_pos);
	free(redir->err_output_pos);
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
