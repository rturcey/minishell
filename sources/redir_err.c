/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_err.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 12:47:40 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/03 16:47:10 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** when parsing encounters wrong char/str (or no char)
** after a redir sign, finds it and prints error
*/

static int		redir_error(t_redir *redir, char *input, int *i, int j)
{
	char	*sym;
	int		k;

	k = 0;
	if (j > *i)
	{
		if (!(sym = malloc(j - *i + 1)))
			return (-1);
		while (*i < j)
			sym[k++] = input[(*i)++];
		sym[k] = '\0';
	}
	else if (input[j] && input[j] == '>' && input[j + 1] == '>')
	{
		if (!(sym = ft_strdup(">>")))
			return (-1);
	}
	else if (!input[j] && !(sym = ft_strdup("newline")))
			return (-1);
	else if (input[j])
	{
		if (!(sym = ft_strdup("c")))
			return (-1);
		sym[0] = input[j];
	}
	dprintf(redir->err_output, "bash: erreur de syntaxe près du symbole inattendu « %s »\n", sym);
	free(sym);
	return (-1);
}

static int		parse_redir(t_redir *redir, char *input, int *i, int *s_fd)
{
	int		j;
	char	*path;
	int		k;

	k = 0;
	path = NULL;
	if (s_fd[1] == 2)
		(*i)++;
	*i = pass_spaces(input, *i);
	j = *i;
	free(sample_str(input, &j, path));
	if (j == *i || is_redir(input, j) != 0)
		return (redir_error(redir, input, i, j));
	return (1);
}

int				find_redir_err(t_redir *redir, char *input, int *i)
{
	int		j;
	int		s_fd[2];

	s_fd[0] = 1;
	j = *i;
	while (is_end(input, j) == 0)
	{
		j = pass_spaces(input, j);
		while (input[j] && (s_fd[1] = is_redir(input, j)) == 0 && ft_isdigit(input[j]) == 0)
			j++;
		if (!input[j])
			return (0);
		if (ft_isdigit(input[j]) != 0)
			s_fd[0] = ft_atoi(&input[j]);
		while (ft_isdigit(input[j]) != 0)
			j++;
		if ((s_fd[1] = is_redir(input, j)) == 0)
			continue;
		j++;
		if (parse_redir(redir, input, &j, s_fd) < 0)
		{
			while (is_end(input, *i) == 0)
				(*i)++;
			return (-1);
		}
	}
	return (0);
}
