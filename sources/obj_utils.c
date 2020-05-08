/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/30 23:23:39 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/08 10:35:53 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** lstnew for arguments
*/

t_arg		*arg_new(void)
{
	t_arg	*new;

	if (!(new = malloc(sizeof(t_arg))))
		return (NULL);
	new->value = NULL;
	new->next = NULL;
	return (new);
}

/*
** lstnew for redirection
*/

t_redir		*redir_new(void)
{
	t_redir	*new;

	if (!(new = malloc(sizeof(t_redir))))
		return (NULL);
	new->cmd_input = 0;
	new->cmd_output = 1;
	new->err_output = 2;
	return (new);
}

/*
** lstnew for object
**(same as plotted on framapad)
*/

t_obj		*obj_new(void)
{
	t_obj	*new;

	if (!(new = malloc(sizeof(t_obj))))
		return (NULL);
	new->obj = NULL;
	new->type = 0;
	new->option = 0;
	if (!(new->args = arg_new()))
	{
		free(new);
		return (NULL);
	}
	new->args_count = 0;
	new->redir = NULL;
	new->result = NULL;
	new->error = NULL;
	new->ret = 0;
	new->next = NULL;
	return (new);
}

/*
**initialize an object, need to add potential redir (see below)
*/

void		init_obj(t_obj *obj, char *sample, int type)
{
	if (!(obj->obj = ft_strdup(sample)))
		return ;
	obj->type = type;
}

/*
**TO DO
**if stock_redir != NULL
**send to a function that parses stock redir and
**puts it in the initialized obj
**and free stock_redir
*/
