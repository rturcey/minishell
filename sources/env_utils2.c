/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/29 11:02:11 by rturcey           #+#    #+#             */
/*   Updated: 2020/04/29 11:59:52 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** returns the value associated to key, or NULL if key isn't in env
*/

char			*find_env_value(char *key, t_env *env)
{
	t_env	*tmp;
	int		i;
	int		len;

	tmp = env;
	len = (int)ft_strlen(key) + 1;
	while (tmp && (i = ft_strncmp(key, tmp->key, len)) != 0)
		tmp = tmp->next;
	if (i == 0)
		return (tmp->value);
	return (NULL);
}
