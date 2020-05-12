/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:30 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/12 22:29:20 by esoulard         ###   ########.fr       */
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

int		sample_quote_cond(char *input, int *i, char **sample, int *j, t_env *env)
{
	int k;
	int l;
	int check;
	int r;
	int heck;

	k = get_next_quote(*sample, *j) - 1;
	l = (*j) - 1;
	heck = 0;
	if (is_quote(input, *i, '\"') == 1)
	{
		while (++l < k)
		{
			if ((*sample)[l] == '\\' && ((*sample)[l + 1] == '\"' ||
				(*sample)[l + 1] == '\\') && k--)
				skim_str(*sample, l - 1, i);
			else if ((*sample)[l] == '$')
			{
				if ((*sample)[l + 1] && (*sample)[l + 1] == '?')
				{
					if (parse_g_err(sample, &l, i) == -1)
						return (-1);
				}
				if ((r = parse_sample_var(sample, &l, env, i)) != -1)
					heck = 1;
				if (r == -2)
					(*j)--;
				else if (r == -1)
					return (-1);
				k = get_next_quote(*sample, *j) - 1;
			}
		}
	}
	l = k - 1;
	check = -1;
	while (++check != 2)
	{
		skim_str(*sample, k, i);
		k = (*j) - 1;
	}
	if (heck == 0)
		(*i) += l - (*j);
	(*j) = l;
	return (0);
}

char	*sample_str(char *input, int *i, char *sample, t_env *env)
{
	int end;
	int j;
	int r;

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
		{
			if (sample_quote_cond(input, i, &sample, &j, env) == -1)
				return (char_free_str(sample));
		}
		else if ((sample[j] == '$') && sample[j + 1] && (sample[j + 1] == '?'))
		{
			if (parse_g_err(&sample, &j, i) == -1)
				return (char_free_str(sample));
		}
		else if (sample[j] == '$')
		{
			if ((r = parse_sample_var(&sample, &j, env, i)) == -2)
				j--;
			else if (r == -1)
				return (char_free_str(sample));
		}
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

int		general_parser(char *input, t_env *env, t_both_env *both_env)
{
	char		*sample;
	int			i;
	t_obj		*obj;
	int			j;
	int			limit = 1;
	int			stock_i;

	if ((lonely_quote(input) == -1) ||
		(last_backslash(input) == -1))
	{
		ft_putstr_fd("bash: multi-line comments not supported\n", 2);
		return (0);
	}
	i = 0;
	i = pass_spaces(input, i);
	while (input[i])
	{
		if (!(obj = obj_new(env)))
			return (-1);
		if (!(obj->redir = redir_new()))
			return (free_obj(obj));
		sample = NULL;
		//il faudra ajouter un moyen de ne verifier les wrong redir que pour chaque bloc de cmd
		if (limit-- == 1)
			find_redir_err(obj, input, &i);
		find_redir(obj, input, &i);
		if (parse_var(input, &i, env, 0) == -1)
		{
			free_obj(obj);
			return (-1);
		}
		if (!input[i])
			continue ;
		stock_i = i;
		if ((sample = sample_str(input, &i, sample, env)) == NULL)
			return (free_obj(obj));
		ft_printf("string sampled [%s]\n", sample);
		if ((j = is_cmd(sample)) != -1)
		{
			init_obj(obj, sample, j);
			if (obj->obj == NULL)
				return (-1);
			if ((j = parse_cmds(obj, input, &i, env)) == -1)
			{
				free_obj(obj);
				free_str(sample);
				return (0);
			}
			if (set_g_err(obj, sample) == 1)
				return (-1);
			free(sample);
		}
		else
		{
			i = stock_i;
			if ((j = parse_exec(obj, input, &i, both_env)) == -1)
			{
				free(sample);
				return (free_obj(obj));
			}
			else if ((j == -2) && (g_err = 127))
			{
				ft_dprintf(2, "%s: command not found\n", sample);
				free(sample);
				free_obj(obj);
				return (0);
			}
		}
		i = pass_spaces(input, i);
		if (obj)
			free_obj(obj);
	}
	return (0);
}
