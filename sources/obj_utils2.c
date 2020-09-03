/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:35:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/03 12:55:39 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		print_result(t_sh *sh, int ret, char *to_free)
{
	if (sh->obj->error)
		ft_putstr_fd(sh->obj->error, sh->obj->redir->err_output);
	else if (sh->obj->result)
		ft_putstr_fd(sh->obj->result, sh->obj->redir->cmd_output);
	free(to_free);
	return (ret);
}

void	maj_err(t_sh *sh, char *str, int err)
{
	if (!sh->obj->error)
		sh->obj->error = str;
	else if (ft_strstr(str, "Permission denied")
		&& !ft_strstr(sh->obj->error, "Permission denied"))
	{
		free(sh->obj->error);
		sh->obj->error = str;
	}
	else if (ft_strncmp(sh->obj->error, "exit\n", 5) == 0)
		sh->obj->error = ft_strjoin_bth(sh->obj->error, str);
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
		free(arg->val);
		free(arg);
		arg = tmp;
	}
	return (NULL);
}

void	*free_redir(t_redir *redir)
{
	if (redir->cmd_in > 0)
		close(redir->cmd_in);
	if (redir->cmd_output != 1 && redir->cmd_output != -1)
		close(redir->cmd_output);
	if (redir->err_output != 2 && redir->err_output != -1)
		close(redir->err_output);
	free(redir);
	return (NULL);
}

int		free_obj(t_obj *obj)
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
	return (-1);
}
