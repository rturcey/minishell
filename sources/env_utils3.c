/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/20 12:18:15 by user42            #+#    #+#             */
/*   Updated: 2020/10/20 13:39:15 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		fix_j(t_env *elt, int *j, char *begin, char *key)
{
	if (elt)
	{
		ft_printf("len valsp %d, valsp |%s|, j = %d\n", (int)ft_strlen(elt->val_sp), elt->val_sp, *j + (int)ft_strlen(elt->val_sp) - 2);
		if (one_quote(begin, key))
			*j = *j + (int)ft_strlen(elt->val_sp) - 3;
		else
			*j = *j + (int)ft_strlen(elt->val) - 2;
		if (elt->sp == 1 || elt->sp == 2)
			(*j)++;
		if (elt->sp == 3)
			*j = *j + 2;
	}
	else
		(*j) -= 2;
}

int				check_var(char *sample)
{
	int		i;

	i = -1;
	if (ft_isdigit(sample[0]))
		return (-1);
	while (sample[++i])
		if (normed_char(sample[i]) == -1)
			return (-1);
	return (0);
}

static int		replace_var(char **sample, char *begin, t_env *elt, char *end)
{
	char	*val;
	int		lev;

	ft_printf("begin = |%s|, end = |%s|\n", begin, end);
	lev = 0;
	if (elt)
	{
		if (one_quote(begin, end) && (lev = 1))
			val = elt->val_sp;
		else
			val = elt->val;
		if (begin[0] && (elt->sp == 1 || elt->sp == 3))
		{
			if (!(*sample = ft_strjoin_sp(begin, ft_strdup(val))))
				return (free_two_str(begin, end));
		}
		else if (!(*sample = ft_strjoin_bth(begin, ft_strdup(val))))
			return (free_two_str(begin, end));
		if (end[0] && !lev && elt->sp > 1)
		{
			if (!(*sample = ft_strjoin_sp(*sample, end)))
				return (free_two_str(*sample, end));
		}
		else if (!(*sample = ft_strjoin_bth(*sample, end)))
			return (free_two_str(*sample, end));
	}
	else
	{
		if (!(*sample = ft_strjoin_bth(begin, end)))
			return (free_two_str(begin, end));
		return (0);
	}
	return (0);
}

int				parse_sample_var(char **sample, int *j, t_env *env, int *r)
{
	char	*key;
	t_env	*elt;
	char	*begin;
	int		i;

	i = ++(*j) + 1;
	if (!(*sample)[*j] || normed_char((*sample)[*j]) == -1)
		return (-2);
	if (!ft_isdigit((*sample)[i - 1]) && (i -= 1))
		while (normed_char((*sample)[i]) == 0)
			i++;
	i -= *j;
	if (!(key = ft_substr(*sample, *j, i)))
		return (-1);
	*r += ft_strlen(key);
	elt = find_env_entry(key, env);
	free(key);
	if (!(begin = ft_substr(*sample, 0, (*j) - 1)))
		return (-1);
	if (!(key = ft_substr(*sample, i + *j, ft_strlen(*sample))))
		return (free_str(begin));
	fix_j(elt, j, begin, key);
	free_str(*sample);
	return (replace_var(sample, begin, elt, key));
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
