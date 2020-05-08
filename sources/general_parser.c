/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:30 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/08 10:51:52 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Haven't used it yet, but will be useful during
**further cmd parsing
*/

int		is_separator(char *str, int i)
{
	if (str[i] == ';' || str[i] == '|')
	{
		if ((i > 0) && (str[i - 1] == '\\'))
			return (0);
		return (1);
	}
	return (0);
}

/*
**Making a clean string, unnecessary quotes removed
*/

void	skim_str(char *sample, int k, int *i)
{
	while (sample[++k] && sample[k + 1])
		sample[k] = sample[k + 1];
	while (sample[k])
		sample[k++] = '\0';
	(*i)++;
}

void	sample_quote_cond(char *input, int *i, char *sample, int *j)
{
	int k;
	int l;
	int check;

	k = get_next_quote(sample, *j) - 1;
	l = (*j) - 1;
	if (is_quote(input, *i, '\"') == 1)
	{
		while (++l < k)
		{
			if (sample[l] == '\\' && sample[l + 1] && k--)
				skim_str(sample, l - 1, i);
		}
	}
	l = k - 1;
	check = -1;
	while (++check != 2)
	{
		skim_str(sample, k, i);
		k = (*j) - 1;
	}
	(*i) += l - (*j);
	(*j) = l;
}

char	*sample_str(char *input, int *i, char *sample)
{
	int end;
	int j;

	if (!input[*i])
		return (NULL);
	end = find_string_end(input, *i);
	if (!(sample = ft_substr(input, *i, (end - *i))))
		return (NULL);
	j = -1;
	while (sample[++j])
	{
		if (sample[j] == '\\')
			skim_str(sample, j - 1, i);
		else if (is_quote(input, *i, 0) == 1)
			sample_quote_cond(input, i, sample, &j);
		(*i)++;
	}
	*i = end;
	return (sample);
}

/*
**First check for lonely quote, if so return error
**enter loop (after passing first spaces), make a
**sample from the first string we can extract from
**input. It is already parsed and clean.
**We strncmp it with the cmds, init an object if
**strcmp is positive, and send obj and input to
**the appropriate function
*/

int		general_parser(char *input, t_env *env)
{
	char	*sample;
	int		i;
	t_obj	*obj;
	int		j;
	int		limit = 1;

	if ((lonely_quote(input) == -1) ||
		(last_backslash(input) == -1))
	{
		ft_putstr_fd("bash: multi-line comments not supported\n", 2);
		return (-1);
	}
	i = 0;
	i = pass_spaces(input, i);
	while (input[i])
	{
		if (!(obj = obj_new()))
			return (-1);
		sample = NULL;
		obj->redir = redir_new();
		//il faudra ajouter un moyen de ne verifier les wrong redir que pour chaque bloc de cmd
		if (limit-- == 1)
			find_redir_err(obj->redir, input, &i);
		find_redir(obj, input, &i);
		if (parse_var(input, &i, env, 0) == -1
			|| (sample = sample_str(input, &i, sample)) == NULL)
		{
			free_obj(obj);
			return (-1);
		}
		ft_printf("string sampled [%s]\n", sample);
		if ((j = is_cmd(sample)) != -1)
		{
			init_obj(obj, sample, j);
			if (obj->obj == NULL)
				return (-1);
			parse_cmds(obj, input, &i, env);
		}
		i = pass_spaces(input, i);
		free(sample);
		if (obj)
			free_obj(obj);
	}
	return (0);
}
