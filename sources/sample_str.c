/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sample_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/03 10:08:03 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/22 10:47:48 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Making a clean string, unnecessary quotes removed
*/

int			skim_str(char *s, int k, int *i)
{
	while (s[++k] && s[k + 1])
		s[k] = s[k + 1];
	while (s[k])
		s[k++] = '\0';
	(*i)++;
	return (1);
}

static int	quote_loop(t_sh *sh, char **s, int *l, int *k)
{
	int		r;
	int		heck;

	if ((heck = 0) == 0 && (*s)[*l + 1] != '$' && ((*s)[*l] == '\\'
	&& ((*s)[*l + 1] == '\"' || (*s)[*l + 1] == '\\') && (*k)--))
		skim_str(*s, *l - 1, sh->tmp);
	else if ((*s)[*l] == '$' && *l > 0 && (*s)[*l - 1] == '\\' && (*k)--)
		skim_str(*s, *l - 2, sh->tmp);
	else if ((*s)[*l] == '$' && normed_char((*s)[*l + 1]) == 0)
	{
		if ((*s)[*l + 1] && (*s)[*l + 1] == '?')
		{
			if (parse_g_err(s, l, sh->tmp) == -1)
				return (-1);
		}
		else if ((r = parse_sample_var(s, l, sh->env, sh->tmp)) != -3)
		{
			if (r == -1)
				return (-1);
			if ((heck = 1) && r == -2)
				(*sh->temp)--;
		}
		*k = get_next_quote(*s, *sh->temp) - 1;
	}
	return (heck);
}

static int	sample_quote_cond(t_sh *sh, int *i, char **s, int *j)
{
	int k;
	int l;
	int heck[2];

	k = get_next_quote(*s, *j) - 1;
	l = (*j) - 1;
	heck[0] = 0;
	if (is_quote(sh->in, *i, '\"') == 1)
	{
		while (++l < k)
		{
			sh->tmp = i;
			sh->temp = j;
			if ((heck[0] = quote_loop(sh, s, &l, &k)) == -1)
				return (-1);
		}
	}
	l = k - 1;
	heck[1] = -1;
	while (++heck[1] != 2 && skim_str(*s, k, i))
		k = (*j) - 1;
	if (heck[0] == 0)
		(*i) += l - (*j);
	(*j) = l;
	return (0);
}

static char	*sample_loop(t_sh *sh, int *i, char **s, int *j)
{
	int		r;

	if ((*s)[*j] == '\\' && (*s)[*j + 1] != '$')
		skim_str(*s, *j - 1, i);
	else if (is_quote(sh->in, *i, 0) == 1)
	{
		if (sample_quote_cond(sh, i, s, j) == -1)
			return (char_free_str(*s));
	}
	else if (((*s)[*j] == '$') && (*s)[*j + 1] && ((*s)[*j + 1] == '?') \
	&& (*j == 0 || (*s)[*j - 1] != '\\'))
	{
		if (parse_g_err(s, j, i) == -1)
			return (char_free_str(*s));
	}
	else if ((*s)[*j] == '$' && *j > 0 && (*s)[*j - 1] == '\\')
		skim_str(*s, *j - 2, i);
	else if ((*s)[*j] == '$' && normed_char((*s)[*j + 1]) == 0)
	{
		if ((r = parse_sample_var(s, j, sh->env, i)) == -2)
			(*j)--;
		else if (r == -1)
			return (char_free_str(*s));
	}
	return (*s);
}

char		*sample_str(t_sh *sh, int *i, char *s)
{
	int end;
	int j;

	if (!sh->in[*i])
		return (NULL);
	end = find_string_end(sh->in, *i);
	if (!(s = ft_substr(sh->in, *i, (end - *i))))
		return (NULL);
	j = -1;
	while (s[++j])
	{
		if (!sample_loop(sh, i, &s, &j))
			return (NULL);
		if (sh->in[*i])
			(*i)++;
	}
	*i = end;
	return (s);
}

/*
**First check for lonely quote, if so return error
**enter loop (after passing first spaces), make a
**s from the first string we can extract from
**in. It is already parsed and clean.
**We strncmp it with the cmds, init an object if
**strcmp is positive, and send obj and in to
**the appropriate function
*/
