/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 08:23:50 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/18 12:19:12 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_permissions(t_stat st)
{
	if (st.st_mode & (S_IXUSR | S_IRWXU))
		return (0);
	return (1);
}

/*
** 0 and path NULL = permission denied
** 0 and path = go exec
** -1 = fatal error return -1 all the way to ms shutdown
** -2 = command not found return -2
*/

static int	sample_path(t_obj *obj, char *sample, char **path)
{
	t_stat	st;
	int		ret;

	ret = 0;
	if ((stat(sample, &st)) == 0 && S_ISDIR(st.st_mode) == 0)
	{
		if (check_permissions(st) == 0)
		{
			if (!(*path = ft_strdup(sample)))
				return (-1);
			//ft_printf("permission ok !\n");
		}
		else
			maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, \
			"Permission denied"), 1);
	}
	else if (((stat(sample, &st)) != 0) && (ret = -2))
		maj_err(obj, ft_sprintf("bash: %s: %s\n", sample,
			strerror(errno)), 1);
	else
		maj_err(obj, ft_sprintf("bash: %s : %s\n", sample, \
		"is a directory"), 1);
	return (print_result(obj, ret, NULL));
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
			//ft_printf("permission ok !\n");
		}
		else
			maj_err(obj, ft_sprintf("bash: %s: %s\n", sample, \
			"Permission denied"), 1);
		free(test);
		return (0);
	}
	free(test);
	return (-2);
}

int			check_path(t_obj *obj, t_env *env, char **path)
{
	char	**path_arr;
	char	*val;
	int		i;
	int		r;

	i = -1;
	if (ft_strchr(obj->obj, '/'))
		return (sample_path(obj, obj->obj, path));
	if (!(val = find_env_val("PATH", env)))
		return (-1);
	if (!(path_arr = ft_split(val, ':')))
		return (free_str(val));
	free(val);
	while (path_arr[++i])
		if ((r = loop_path(path_arr[i], path, obj->obj, obj)) != -2)
		{
			if (r == -1)
				return (free_array(path_arr, -1, -1));
			break ;
		}
	free_array(path_arr, -1, -1);
	return (print_result(obj, r, NULL));
}
