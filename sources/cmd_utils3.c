/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/19 10:10:47 by rturcey           #+#    #+#             */
/*   Updated: 2020/07/27 09:46:04 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Cmd array init and strncmp with sample from input
**return appropriate index (corresponding to cmd function)
*/

int			is_cmd(char *sample)
{
	char	*cmds[8];
	int		j;

	cmds[0] = "echo";
	cmds[1] = "cd";
	cmds[2] = "pwd";
	cmds[3] = "export";
	cmds[4] = "unset";
	cmds[5] = "env";
	cmds[6] = "exit";
	cmds[7] = NULL;
	j = -1;
	while (cmds[++j] != NULL)
	{
		if ((ft_strlen(cmds[j]) == ft_strlen(sample)) &&
			ft_strncmp(cmds[j], sample, ft_strlen(sample)) == 0)
			return (j);
	}
	return (-1);
}

/*
**Cmds parsing functions array, and sends us to
**function corresponding to the matching index
**we got from strncmp
*/

int			parse_cmds(t_obj *obj, char *input, int *i, t_env *env)
{
	t_parse_cmd parse_cmd[7];
	int			ret;

	parse_cmd[0] = parse_echo;
	parse_cmd[1] = parse_cd;
	parse_cmd[2] = parse_pwd;
	parse_cmd[3] = parse_export;
	parse_cmd[4] = parse_unset;
	parse_cmd[5] = parse_env;
	parse_cmd[6] = parse_exit;
	if ((ret = parse_cmd[obj->type](obj, input, i, env)) == -1)
	{
		while (is_end(input, *i) == 0)
			(*i)++;
		return (-1);
	}
	return (ret);
}

void		pass_option(t_obj *obj, char *input, int *i)
{
	while (is_end(input, *i) == 0)
	{
		if (ft_strncmp("-n", &input[*i], 2) == 0
			&& (is_end(input, *i + 2) == 1 || is_space(input, *i + 2) == 1)
			&& ((*i) += 2))
			obj->option = 1;
		else
			return ;
		*i = pass_spaces(input, *i);
	}
}
