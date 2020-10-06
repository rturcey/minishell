/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/29 12:00:56 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/06 14:13:31 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pass_quotes(t_sh *sh, int *i)
{
	if (sh->in[*i] == '\"' && ++(*i))
		while (sh->in[*i] && sh->in[*i] != '\"')
			(*i)++;
	else if (sh->in[*i] == '\'' && ++(*i))
		while (sh->in[*i] && sh->in[*i] != '\'')
			(*i)++;
}

static int	pass_content(t_sh *sh, int *i)
{
	char	*sample;

	sample = NULL;
	while (sh->in[*i] && is_end(sh->in, *i) == 0)
	{
		if (is_redir(sh->in, *i) && ++(*i))
		{
			if (is_redir(sh->in, *i - 1) == 2)
				++(*i);
			*i = pass_spaces(sh->in, *i);
			if (!sh->in[*i] || is_redir(sh->in, *i) || is_end(sh->in, *i))
			{
				if (sh->in[*i] && !(sample = ft_substr(sh->in, *i, 1)))
					return (-2);
				else if (!sample && !(sample = ft_strdup("\\n")))
					return (-2);
				g_err = 2;
				ft_dprintf(2, "bash: parse error near `%s\'\n", sample);
				return (free_str(sample));
			}
		}
		pass_quotes(sh, i);
		(*i)++;
	}
	return (0);
}

static int	sample_err(t_sh *sh, int j, int i, char **sample)
{
	if (sh->in[j + 1] && is_end(sh->in, j + 1) == 1)
	{
		if (!(*sample = ft_substr(sh->in, j, 2)))
			return (-2);
	}
	else if (j == i)
	{
		if (!(*sample = ft_substr(sh->in, j, 1)))
			return (-2);
	}
	else if (is_end(sh->in, j = (pass_spaces(sh->in, j + 1))) == 1
	&& sh->in[j])
	{
		if (!(*sample = ft_substr(sh->in, j, 1)))
			return (-2);
	}
	else
		j = -1;
	return (j);
}

int			parse_syntax(t_sh *sh, int i)
{
	int		j;
	int		k;
	char	*sample;

	j = i;
	while (sh->in[j])
	{
		if (pass_content(sh, &j) == -1)
			return (-1);
		if (sh->in[j] && is_end(sh->in, j) == 1)
		{
			if ((k = sample_err(sh, j, i, &sample)) == -2)
				return (-2);
			if (k >= 0 && (g_err = 2))
			{
				ft_dprintf(2, \
				"bash: syntax error near unexpected token `%s\'\n"\
				, sample);
				return (free_str(sample));
			}
		}
		if (sh->in[j])
			++j;
	}
	return (0);
}
