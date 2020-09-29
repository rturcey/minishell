/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 10:03:58 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/29 13:04:37 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		start_exec(char *tmp, t_sh *sh, int *i)
{
	if (dup_exec(sh) == -1)
		return (-1);
	if (!tmp)
	{
		if (parse_cmds(sh, i) == -1)
			return (-1);
		exit(g_err);
	}
	else
	{
		execve(tmp, sh->obj->args, sh->obj->charenv);
		if (errno != 2)
			maj_err(sh, ft_sprintf("bash: %s: %s\n", sh->obj->obj, \
			strerror(errno)), 2);
		exit(print_result(sh, g_err, NULL));
	}
	return (0);
}
