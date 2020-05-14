/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 08:23:50 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/14 10:17:25 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int			check_permissions(t_stat *st)
{
	if (S_IXUSR(st.st_mode) || S_IRWXU(st.st_mode))
		return (0);
	return (1);
}

int			check_path(t_obj *obj, char *sample, t_env *env, char **path)
{
	char	**path_arr;
	char	*test;
	int		i;
	t_stat	*st;

	i = -1;
	/*if (ft_strchr(sample, '/'))
		parse_path*/
	if (!(path_arr = ft_split(find_env_value("PATH", env))))
		return (-1);
	while (path_arr[++i])
	{
		if (!(test = ft_strjoin_nofree(path_arr[i], sample)))
			return (free_array(path_arr, -1));
		if (!(st = malloc(sizeof(t_stat))))
			return (free_array_and_str(path_arr, -1, test));
		if ((st = stat(test)) == 0 && S_ISDIR(st.st_mode) == 0)
		{
			if (check_permissions() == 0)
				printf("permission ok !\n");
			else
				printf("permission not ok !\n");
		}
		free(st);
		free(test);
	}
	return (0);
}
