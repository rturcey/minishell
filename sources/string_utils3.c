/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/05 19:52:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/10/06 09:20:32 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		in_quotes(char *str, int i)
{
	char	tmp;
	int		j;
	int		count;

	j = i;
	count = 0;
	tmp = 0;
	while (--j > 0)
	{
		if (count == 0 && is_quote(str, j, 0) == 1)
		{
			tmp = str[j];
			count++;
		}
		else if (str[j] == tmp)
			count++;
	}
	if (count % 2 != 0)
		return (1);
	return (0);
}

char	*get_pwd(t_env *env)
{
	char	*workdir;

	if (!(workdir = find_env_val("PWD", env))[0])
	{
		free(workdir);
		if (!(workdir = ft_calloc(PATH_MAX, 1)))
			return (NULL);
		if (getcwd(workdir, PATH_MAX) == NULL)
			return (char_free_str(workdir));
	}
	return (workdir);
}
