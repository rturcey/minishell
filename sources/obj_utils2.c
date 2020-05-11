/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:35:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/11 21:07:39 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_result(t_obj *obj, int ret, char *to_free)
{
	if (obj->error)
		ft_putstr_fd(obj->error, obj->redir->err_output);
	else if (obj->result)
		ft_putstr_fd(obj->result, obj->redir->cmd_output);
	free(to_free);
	return (ret);
}

void	maj_err(t_obj *obj, char *str, int err)
{
	if (!obj->error)
		obj->error = str;
	else if (ft_strstr(str, "Permission denied")
		&& !ft_strstr(obj->error, "Permission denied"))
	{
		free(obj->error);
		obj->error = str;
	}
	else if (ft_strncmp(obj->error, "exit\n", 5) == 0)
		obj->error = ft_strjoin_bth(obj->error, str);
	else
		free(str);
	g_err = err;
}

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
		free(obj->error);
		free(obj->result);
		free(obj);
		obj = tmp;
	}
	return (NULL);
}
