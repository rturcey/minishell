/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 08:23:50 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/14 12:27:31 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_permissions(t_stat st)
{
	if (st.st_mode & (S_IXUSR | S_IRWXU))
		return (0);
	return (1);
}

static int	sample_path(t_obj *obj, char *sample, char **path)
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
			maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, \
			"Permission denied"), 1);
	}
	else if ((stat(sample, &st)) != 0)
		maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, strerror(errno)), 1);
	else
		maj_err(obj, ft_sprintf("bash: %s : %s\n", sample, \
		"is a directory"), 1);
	free(sample);
	return (print_result(obj, -1, NULL));
}

static int	loop_path(char *path_str, char **path, char *sample, t_obj *obj)
{
	t_stat	st;
	char	*test;

	if (!(test = ft_strjoin_slash(path_str, sample)))
		return (-1);
	if ((stat(test, &st)) == 0 && S_ISDIR(st.st_mode) == 0)
	{
		if (check_permissions(st) == 0)
		{
			if (!(*path = ft_strdup(test)))
				return (free_two_str(path_str, test));
			printf("permission ok !\n");
		}
		else
			maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, \
			"Permission denied"), 1);
		free(test);
		return (1);
	}
	free(test);
	return (0);
}

int			check_path(t_obj *obj, char *sample, t_env *env, char **path)
{
	char	**path_arr;
	char	*value;
	int		i;
	int		r;

	i = -1;
	if (ft_strchr(sample, '/'))
		return (sample_path(obj, sample, path));
	if (!(value = find_env_value("PATH", env)))
		return (free_str(sample));
	if (!(path_arr = ft_split(value, ':')))
		return (free_two_str(value, sample));
	free(value);
	while (path_arr[++i])
		if ((r = loop_path(path_arr[i], path, sample, obj)) != 0)
		{
			if (r == -1)
				return (free_array_and_str(path_arr, -1, sample));
			break ;
		}
	free_array(path_arr, -1, -1);
	return (print_result(obj, -1, sample));
}
