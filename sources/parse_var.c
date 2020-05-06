/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 11:12:13 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/06 11:33:26 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_keyvar(char *input, int *i)
{
	int		j;
	t_env	*elt;

	j = *i;
	elt = NULL;
	while (input[j] && is_space(input[j]) == 0 && input[j] != '=')
		j++;
	if (!input[j] || input[j] != '=')
		return (NULL);
	*i = pass_spaces(input, *i);
	if (is_end(input[*i]) != 0)

}

int		parse_var(char *input, int *i, t_env *env)
{
	while (is_end(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		if (find_redir(obj->redir, input, i) < 0)
			return (-1);

	}
}
