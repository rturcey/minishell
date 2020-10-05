/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 13:23:14 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/05 12:13:24 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int				pluseq(char *sample, int i)
{
	if (sample[i] == '+' && sample[i + 1] && sample[i + 1] == '=')
		return (1);
	return (0);
}

int				repluseq(char *sample, int i, t_env *env, int in)
{
	char	*key;
	char	*val;
	t_env	*elt;

	if (ft_isalnum(sample[i + 2]) == 0)
		return (-1);
	if (!(key = ft_substr(sample, 0, i)))
		return (-2);
	i += 2;
	if (!(val = ft_strdup(&sample[i])))
		return (-2);
	if (!(elt = find_env_entry(key, env)))
	{
		if (!(elt = env_new(in)))
			return (-2);
		elt->key = key;
		elt->val = val;
		export_var(elt, env, in);
		del_var(elt);
	}
	else if (free_str(key) == -1
	&& !(elt->val = ft_strjoin_bth(elt->val, val)))
		return (-2);
	return (0);
}

int				old_pwd(t_env *env)
{
	t_env	*old;
	char	*workdir;
	int		ret;

	old = env_new(1);
	if (!(workdir = ft_calloc(PATH_MAX, 1)))
		return (-1);
	ret = 0;
	if (getcwd(workdir, PATH_MAX) == NULL)
		return (free_str(workdir));
	if (!(old->val = workdir))
		return (-1);
	if (!(old->key = ft_strdup("OLDPWD")))
		return (free_str(workdir));
	add_var(old, env);
	del_var(old);
	return (ret);
}
