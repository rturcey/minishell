/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:29:07 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/03 14:12:35 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**DUMMY PARSING FUNCTIONS
*/

int		parse_pwd(t_obj *obj, char *input, int *i, t_env *env)
{
	(void)env;
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in pwd\n");
	return (0);
}

int		parse_export(t_obj *obj, char *input, int *i, t_env *env)
{
	(void)env;
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in export\n");
	return (0);
}

int		parse_unset(t_obj *obj, char *input, int *i, t_env *env)
{
	(void)env;
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in unset\n");
	return (0);
}

int		parse_env(t_obj *obj, char *input, int *i, t_env *env)
{
	int		ret;

	while (input[*i] && is_separator(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		if ((ret = find_redir(obj->redir, input, i)) == 0)
		{
			while (*i < (int)ft_strlen(input))
				if (find_redir(obj->redir, input, i) == 0)
					(*i)++;
			ft_putstr_fd("env: too many arguments\n", obj->redir->err_output);
			return (-1);
		}
	}
	print_env(env, obj->redir->cmd_output);
	return (0);
}

int		parse_exit(t_obj *obj, char *input, int *i, t_env *env)
{
	(void)env;
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in exit\n");
	return (0);
}
