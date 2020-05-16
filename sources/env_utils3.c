/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 10:25:57 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/16 11:39:54 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		normed_char(char c)
{
	if (ft_isalnum(c) == 0)
		if (c != '-' && c != '_')
			return (-1);
	return (0);
}

/*
** for unset
*/

int				check_var(char *sample)
{
	int		i;

	i = -1;
	while (sample[++i])
		if (normed_char(sample[i]) == -1)
			return (-1);
	return (0);
}

static int		replace_var(char **sample, char *begin, char *value, char *end)
{
	if (!(*sample = ft_strjoin_bth(begin, value)))
	{
		free(end);
		return (free_two_str(begin, value));
	}
	if (!(*sample = ft_strjoin_bth(*sample, end)))
		return (free_two_str(*sample, end));
	return (0);
}

int				parse_sample_var(char **sample, int *j, t_env *env, int *r)
{
	char	*key;
	char	*value;
	char	*begin;
	int		i;

	(*j)++;
	i = *j;
	if (!(*sample)[*j] || normed_char((*sample)[*j]) == -1)
		return (-2);
	while (normed_char((*sample)[i]) == 0)
		i++;
	i -= *j;
	if (!(key = ft_substr(*sample, *j, i)))
		return (-1);
	*r += ft_strlen(key);
	if (!(value = find_env_value(key, env)))
		return (-2);
	free(key);
	if (!(begin = ft_substr(*sample, 0, (*j) - 1)))
		return (free_str(value));
	if (!(key = ft_substr(*sample, i + *j, ft_strlen(*sample))))
		return (free_two_str(value, begin));
	free(*sample);
	*j = *j + ft_strlen(value) - 2;
	return (replace_var(sample, begin, value, key));
}

char			**env_to_array(t_env *env)
{
	int		i;
	t_env	*tmp;
	char	**arr;

	i = 0;
	tmp = env;
	while (env && (++i))
		env = env->next;
	env = tmp;
	if (!(arr = malloc(++i * sizeof(char *))))
		return (NULL);
	i = 0;
	while (env)
	{
		if (!(arr[i++] = ft_strjoin_dic(env->key, env->value)))
		{
			free_array(arr, i - 1, -1);
			return (NULL);
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
