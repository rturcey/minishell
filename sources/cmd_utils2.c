/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 02:29:07 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/01 12:54:06 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**DUMMY PARSING FUNCTIONS
*/

int		parse_pwd(t_obj *obj, char *input, int *i)
{
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in pwd\n");
	return (0);
}

int		parse_export(t_obj *obj, char *input, int *i)
{
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in export\n");
	return (0);
}

int		parse_unset(t_obj *obj, char *input, int *i)
{
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in unset\n");
	return (0);
}

int		parse_env(t_obj *obj, char *input, int *i)
{
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in env\n");
	return (0);
}

int		parse_exit(t_obj *obj, char *input, int *i)
{
	(void)obj;
	(void)input;
	(void)i;
	ft_printf("in exit\n");
	return (0);
}
