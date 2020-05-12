/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   err_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/11 21:28:11 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/12 14:19:37 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		set_g_err(t_obj *obj, char *sample)
{
	if (!obj || !obj->obj)
		return (0);
	if (obj && obj->obj && (strncmp(obj->obj, "exit",
		ft_strlen(obj->obj)) == 0) && !ft_strstr(obj->error,
		"too many"))
	{
		free(sample);
		free_obj(obj);
		return (1);
	}
	if (!(obj->error))
		g_err = 0;
	return (0);
}

int		parse_g_err(char **sample, int *l, int *i)
{
	char	*value;
	char	*begin;
	char	*end;

	skim_str(*sample, (*l) - 1, i);
	skim_str(*sample, (*l) - 1, i);
	if (!(value = ft_itoa(g_err)))
		return (-1);
	if (!(begin = ft_substr(*sample, 0, (*l))))
		return (free_str(value));
	if (!(end = ft_substr(*sample, *l, ft_strlen(*sample))))
		return (free_two_str(value, begin));
	free(*sample);
	*i += ft_strlen(value);
	if (!(*sample = ft_strjoin_bth(begin, value)))
	{
		free(end);
		return (free_two_str(begin, value));
	}
	if (!(*sample = ft_strjoin_bth(*sample, end)))
		return (free_two_str(*sample, end));
	return (0);
}
