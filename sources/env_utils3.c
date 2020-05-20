/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/09 10:25:57 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/20 11:26:58 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				normed_char(char c)
{
	if (ft_isalnum(c) == 0)
		if (c != '-' && c != '_' && c != '?')
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

static int		replace_var(char **sample, char *begin, char *val, char *end)
{
	if (!(*sample = ft_strjoin_bth(begin, val)))
	{
		free(end);
		return (free_two_str(begin, val));
	}
	if (!(*sample = ft_strjoin_bth(*sample, end)))
		return (free_two_str(*sample, end));
	return (0);
}

int				parse_sample_var(char **sample, int *j, t_env *env, int *r)
{
	char	*key;
	char	*val;
	char	*begin;
	int		i;

	(*j)++;
	i = *j;
	if (!(*sample)[*j] || normed_char((*sample)[*j]) == -1)
		return (-2);
	if (!ft_isdigit((*sample)[i]))
		while (normed_char((*sample)[i]) == 0)
			i++;
	else
		i++;
	i -= *j;
	if (!(key = ft_substr(*sample, *j, i)))
		return (-1);
	*r += ft_strlen(key);
	if (!(val = find_env_val(key, env)))
		return (-2);
	free(key);
	if (!(begin = ft_substr(*sample, 0, (*j) - 1)))
		return (free_str(val));
	if (!(key = ft_substr(*sample, i + *j, ft_strlen(*sample))))
		return (free_two_str(val, begin));
	free(*sample);
	*j = *j + ft_strlen(val) - 2;
	return (replace_var(sample, begin, val, key));
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
		if (!(arr[i++] = ft_strjoin_dic(env->key, env->val)))
		{
			free_array(arr, i - 1, -1);
			return (NULL);
		}
		env = env->next;
	}
	arr[i] = NULL;
	return (arr);
}
