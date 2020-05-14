/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 08:23:50 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/14 12:03:30 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			check_permissions(t_stat st)
{
	if (st.st_mode & (S_IXUSR | S_IRWXU))
		return (0);
	return (1);
}

int			sample_path(t_obj *obj, char *sample, char **path)
{
	t_stat	st;

	if ((stat(sample, &st)) == 0 && S_ISDIR(st.st_mode) == 0)
	{
		if (check_permissions(st) == 0)
			{
				if (!(*path = ft_strdup(sample)))
					return (free_str(sample));
				printf("permission ok !\n");
			}
			else
				maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, "Permission denied"), 1);
	}
	else if ((stat(sample, &st)) != 0)
		maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, strerror(errno)), 1);
	else
		maj_err(obj, ft_sprintf("bash: %s : %s\n", sample, "is a directory"), 1);
	free(sample);
	return (print_result(obj, -1, NULL));
}

int			check_path(t_obj *obj, char *sample, t_env *env, char **path)
{
	char	**path_arr;
	char	*test;
	int		i;
	t_stat	st;

	i = -1;
	(void)obj;
	(void)path;
	if (ft_strchr(sample, '/'))
		return (sample_path(obj, sample, path));
	test = find_env_value("PATH", env);
	if (!(path_arr = ft_split(test, ':')))
		return (free_str(test));
	free(test);
	while (path_arr[++i])
	{
		if (!(test = ft_strjoin_slash(path_arr[i], sample)))
			return (free_array(path_arr, -1, -1));
		if ((stat(test, &st)) == 0 && S_ISDIR(st.st_mode) == 0)
		{
			if (check_permissions(st) == 0)
			{
				if (!(*path = ft_strdup(test)))
					return (free_array_and_str(path_arr, -1, test));
				printf("permission ok !\n");
			}
			else
				maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, "Permission denied"), 1);
			break;
		}
		free(test);
	}
	free_array(path_arr, -1, -1);
	free(sample);
	return (print_result(obj, -1, test));
}
