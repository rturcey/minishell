/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/11 21:28:11 by esoulard          #+#    #+#             */
/*   Updated: 2020/07/29 11:25:45 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		set_g_err(t_sh *sh)
{
	if (!sh->obj || !sh->obj->obj)
		return (0);
	if (sh->obj && sh->obj->obj && (strncmp(sh->obj->obj, "exit",
		ft_strlen(sh->obj->obj)) == 0) && !ft_strstr(sh->obj->error,
		"too many"))
		return (1);
	if (!(sh->obj->error))
		sh->err = 0;
	return (0);
}

int		parse_g_err(t_sh *sh, char **sample, int *l, int *i)
{
	char	*val;
	char	*begin;
	char	*end;

	skim_str(*sample, (*l) - 1, i);
	skim_str(*sample, (*l) - 1, i);
	if (!(val = ft_itoa(sh->err)))
		return (-1);
	if (!(begin = ft_substr(*sample, 0, (*l))))
		return (free_str(val));
	if (!(end = ft_substr(*sample, *l, ft_strlen(*sample))))
		return (free_two_str(val, begin));
	free(*sample);
	*i += ft_strlen(val) - 1;
	if (!(*sample = ft_strjoin_bth(begin, val)))
	{
		free(end);
		return (free_two_str(begin, val));
	}
	if (!(*sample = ft_strjoin_bth(*sample, end)))
		return (free_two_str(*sample, end));
	return (0);
}
