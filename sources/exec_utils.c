/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 14:06:46 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/13 14:50:00 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** So far, the program launches but minishell crashes when it shuts.
** I need to look into forking and processes and stuff.
*/

int		try_exec(char *tmp, char **av, char **env, int fr)
{
	int wstatus;

	wstatus = 0;
	if (execve(tmp, av, env) == 0)
		wait(&wstatus);
	else
	{
		if (fr == 1)
			free(tmp);
		return (-1);
	}
	return (0);
}

/*
** Preparing for execve.
** If execve doesn't work with just the sample we got, we need to check
** if the program is found at the end of any path in PATH (they're separated
** with ':'). We split the paths, gather them in a char **path, and go through
** the different path + ('/' + tmp) combinations, trying to exec each one.
*/

int		prep_exec(char *tmp, char **av, t_env *env, char **b_env)
{
	char	**path;
	char	*tmp_cp;
	int		i;

	if ((try_exec(tmp, av, b_env, 0) == -1) && (i = -1))
	{
		tmp_cp = find_env_value("PATH", env);
		if (!(path = ft_split(tmp_cp, ':')) && (free_str(tmp_cp) == -1))
			return (free_array_and_str(av, -1, tmp));
		free_str(tmp_cp);
		if (!(tmp_cp = ft_strjoin_bth(ft_strdup("/"), tmp)) &&
			(free_str(tmp) == -1))
			return (free_two_arrays(av, path));
		while (path[++i] != NULL)
		{
			if (!(tmp = ft_strjoin_bth(ft_strdup(path[i]), ft_strdup(tmp_cp))))
				return (free_two_arr_and_str(path, av, tmp_cp, -1));
			if (try_exec(tmp, av, b_env, 1) == 0)
				return (free_two_arr_two_str(av, path, tmp, tmp_cp));
		}
		free_two_arr_and_str(path, av, tmp_cp, -1);
	}
	else
		free_array_and_str(av, -1, tmp);
	return (-2);
}

/*
** Putting potential params for the program in a char **av
*/

char	**conv_av(char *input, int *i, t_obj *obj, t_env *env)
{
	int		j;
	char	**av;
	int		r;
	int		k;

	if (!(av = malloc(sizeof(char *) * (count_strings(input, *i) + 1))))
		return (NULL);
	k = *i;
	j = 0;
	r = 0;
	while (is_end(input, *i) == 0)
	{
		if (redir_loop(obj, input, &k) == -1)
			return (NULL);
		if ((is_end(input, k) == 1) && (r = 1))
			break ;
		if (!(av[j] = sample_str(input, &k, av[j], env)))
			return (NULL);
		j++;
	}
	if (r == 1)
		av[j] = NULL;
	else
		av[j + 1] = NULL;
	return (av);
}

/*
** Had to add a structure containing both our env and bash env for lack of space
** (execve uses bash env as a param) (both_env is init in main).
** We make a sample with the potential program's
** name, make an argv with potential following args for the executable.
** We return -2 if no program was found, -1 in case of a fatal error.
*/

int		parse_exec(t_obj *obj, char *input, int *i, t_env *env)
{
	char	**av;
	char	*tmp;
	int		stock_i;
	int		r;
	char	**b_env;

	stock_i = *i;
	tmp = NULL;
	if (!(tmp = sample_str(input, i, tmp, env)))
		return (-1);
	av = NULL;
	*i = stock_i;
	if (!(b_env = env_to_array(env)))
		return (free_str(tmp));
	if (!(av = conv_av(input, i, obj, env)))
	{
		*i = stock_i;
		return (free_two_arr_and_str(av, b_env, tmp, -1));
	}
	if ((((r = prep_exec(tmp, av, env, b_env)) == -1) || (r == -2)))
	{
		*i = stock_i;
		return (free_array(b_env, -1, r));
	}
	return (free_two_arr_and_str(av, b_env, tmp, 0));
}
