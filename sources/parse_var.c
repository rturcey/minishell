/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 11:12:13 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/09 11:40:29 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		check_vars(char *input, int *i, t_env *env)
{
	int		j;
	char	*sample;

	sample = NULL;
	while (is_end(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		j = *i;
		while (is_end(input, j) == 0 && is_space(input, j) == 0
			&& input[j] != '=')
			j++;
		if (input[j] != '=')
			return (-1);
		*i = j + 1;
		*i = pass_spaces(input, *i);
		if (is_end(input, *i) != 0)
			return (0);
		if (*i != j + 1)
			return (-1);
		if (!(sample = sample_str(input, i, sample, env)))
			return (-1);
		free(sample);
		*i = pass_spaces(input, *i);
	}
	return (0);
}

int				parse_var(char *input, int *i, t_env *env, int len)
{
	char	*to_split;
	char	**env_new;
	t_env	*wagon;
	t_env	*begin;

	if (check_vars(input, i, env) == -1)
		return (0);
	while (is_end(input, len) == 0)
		len++;
	if (!(to_split = ft_substr(input, 0, len)))
		return (-1);
	if (!(env_new = ft_split(to_split, ' ')))
		return (free_str(to_split));
	if (!(wagon = init_env(env_new, 0)))
		return (free_array_and_str(env_new, to_split) == 2);
	begin = wagon;
	while (wagon)
	{
		if (find_env_entry(wagon->key, env))
			wagon->in = 1;
		add_var(wagon, env);
		wagon = wagon->next;
	}
	free_array_and_str(env_new, to_split);
	return (env_clear(begin));
}
