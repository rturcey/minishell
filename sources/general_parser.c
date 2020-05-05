/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:30 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/05 10:45:42 by rturcey          ###   ########.fr       */
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
	int		limit = 1;

	if (lonely_quote(input) == -1)
	{
		ft_putstr_fd("bash: multiligne non géré\n", 2);
		return (-1);
	}
	i = 0;
	i = pass_spaces(input, i);
	while (input[i])
	{
		obj = NULL;
		sample = NULL;
		redir = redir_new();
		//il faudra ajouter un moyen de ne verifier les wrong redir que pour chaque bloc de cmd
		if (limit-- == 1)
			find_redir_err(redir, input, &i);
		find_redir(redir, input, &i);
		if ((sample = sample_str(input, &i, sample)) == NULL)
		{
			free_redir(redir);
			return (-1);
		}
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
		if (obj)
			free_obj(obj);
		else
			free_redir(redir);
	}
	return (0);
}
