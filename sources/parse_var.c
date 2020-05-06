/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 11:12:13 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/06 11:53:48 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*ft_keyvar(char *input, int *i, t_env *env)
{
	int		j;
	t_env	*elt;

	j = *i;
	elt = NULL;
	while (input[j] && is_space(input, j) == 0 && input[j] != '=')
		j++;
	if (!input[j] || input[j] != '=')
		return (NULL);
	*i = pass_spaces(input, *i);
	if (is_end(input, *i) != 0)
	{
		elt=env_new(0);
		add_var(elt, env);
	}
	//sampler
	return (elt);
}

int		parse_var(char *input, int *i, t_env *env, t_redir *redir)
{
	(void)env;
	while (is_end(input, *i) == 0)
	{
		*i = pass_spaces(input, *i);
		if (find_redir(redir, input, i) < 0)
			return (-1);
	}
	return (0);
}
