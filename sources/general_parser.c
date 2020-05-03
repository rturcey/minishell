/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/01 00:09:24 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/03 13:27:18 by esoulard         ###   ########.fr       */
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

void 	skim_str(char *sample, int k)
{
	while (sample[++k] && sample[k + 1])
		sample[k] = sample[k + 1];
	while (sample[k])
		sample[k++] = '\0';
}

char	*sample_str(char *input, int *i, char *sample)
{
	int end;
	int j;
	int k;
	int check;

	if (!input[*i])
		return (NULL);
	end = find_string_end(input, *i);
	if (!(sample = ft_substr(input, *i, (end - *i))))
		return (NULL);
	j = -1;
	j = -1;
	while (sample[++j])
	{
		if (is_quote(sample, j, 0) == 1)
		{
			k = get_next_quote(sample, j) - 1;
			*i = k - 1;
			check = 0;
			while (check != 2)
			{
				skim_str(sample, k);
				k = j - 1;
				check++;
			}
			j = *i;
		}
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
	t_redir	*redir;

	if (lonely_quote(input) == -1)
		return (-1);
	i = 0;
	i = pass_spaces(input, i);
	while (input[i])
	{
		obj = NULL;
		sample = NULL;
		redir = redir_new();
		find_redir(redir, input, &i);
		if ((sample = sample_str(input, &i, sample)) == NULL)
			return (-1);
		ft_printf("string sampled [%s]\n", sample);
		if ((j = is_cmd(sample)) != -1)
		{
			obj = init_obj(sample, j, redir);
			if (obj == NULL)
				return (-1);
			parse_cmds(obj, input, &i, env);
		}
		i = pass_spaces(input, i);
		free(sample);
		free_redir(redir);
		if (obj)
			free_obj(obj);
	}
	return (0);
}
