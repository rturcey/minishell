/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 12:11:50 by rturcey           #+#    #+#             */
/*   Updated: 2020/07/21 13:30:34 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				make_tab(t_redir *redir)
{
	int		i;

	i = -1;
	free(redir->cmd_input);
	if (!(redir->cmd_input = malloc((redir->incount + 1) * sizeof(int))))
		return (-1);
	while (++i < redir->incount + 1)
		redir->cmd_input[i] = -2;
	return (0);
}

int				is_end(char *str, int i)
{
	if (!str[i] || is_separator(str, i) == 1)
		return (1);
	return (0);
}

/*
** returns 1 if >, 2 if >>, 3 if <
*/

static int		quote_redir(char *str, int i)
{
	if (i > 1 && (ft_strncmp(&str[i - 2], "\'>>", 3) == 0
		|| ft_strncmp(&str[i - 2], "\">>", 3) == 0))
		return (1);
	if (i > 0 && (ft_strncmp(&str[i - 1], "\'>", 2) == 0
		|| ft_strncmp(&str[i - 1], "\'>>", 3) == 0
		|| ft_strncmp(&str[i - 1], "\'<", 2) == 0
		|| ft_strncmp(&str[i - 1], "\">", 2) == 0
		|| ft_strncmp(&str[i - 1], "\">>", 3) == 0
		|| ft_strncmp(&str[i - 1], "\"<", 2) == 0))
		return (1);
	if (ft_strncmp(&str[i], "\'>", 2) == 0
		|| ft_strncmp(&str[i], "\'>>", 3) == 0
		|| ft_strncmp(&str[i], "\'<", 2) == 0
		|| ft_strncmp(&str[i], "\">", 2) == 0
		|| ft_strncmp(&str[i], "\">>", 3) == 0
		|| ft_strncmp(&str[i], "\"<", 2) == 0)
		return (1);
	return (0);
}

int				is_redir(char *str, int i)
{
	if (quote_redir(str, i) == 1)
		return (0);
	if (str[i] == '>' || str[i] == '<')
	{
		if ((i > 0) && (str[i - 1] == '\\'))
			return (0);
		if (str[i] == '>' && str[i + 1] == '>')
			return (2);
		if (str[i] == '>')
			return (1);
		return (3);
	}
	return (0);
}
