/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 14:06:46 by esoulard          #+#    #+#             */
/*   Updated: 2020/07/21 14:08:17 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** So far, the program launches but minishell crashes when it shuts.
** I need to look into forking and processes and stuff.
*/

int		try_exec(char *tmp, char **av, char **env, t_obj *obj)
{
	pid_t	pid;
	int		status;
	int		i;

	i = 0;
	if ((pid = fork()) < 0)
	{
		ft_dprintf(2, ft_sprintf("fork: %s\n", strerror(errno)));
		return (-1);
	}
	else if (pid == 0)
	{
		//ft_printf("Child process.  ");
		if ((dup2(obj->redir->cmd_output, 1) == -1) ||
			dup2(obj->redir->err_output, 2) == -1)
			return (-1);
		if (obj->redir->incount > 0)
		{
			while (obj->redir->cmd_input[i] == -4)
				i++;
			if (i < obj->redir->incount)
			{
				if (dup2(obj->redir->cmd_input[i], 0) == -1)
					return (-1);
				close(obj->redir->cmd_input[i]);
				obj->redir->cmd_input[i] = -4;
				execve(tmp, av, env);
			}
		}
		else
			execve(tmp, av, env);
	}
	else
	{
		ft_printf("PARENT started with pid=%d.\n", (int)pid);
		status = 0;
		wait(&status);
		g_err = status;
		ft_printf("PARENT resumed, status code: %d. Terminating\n", status);
	}
	return (0);
}

/*
** Putting potential params for the program in a char **av
*/

char	**conv_av(char *input, int *i, t_obj *obj, t_env *env)
{
	int		j;
	char	**av;

	if (!(av = malloc(sizeof(char *) * (count_strings(input, *i) + 2))))
		return (NULL);
	j = 0;
	if (!(av[j++] = ft_strdup(obj->obj)))
		return (char_free_array(av, 0));
	while (is_end(input, *i) == 0)
	{
		if (redir_loop(obj, input, i) == -1)
			return (char_free_array(av, j));
		if ((is_end(input, *i) == 1))
			break ;
		if (!(av[j] = sample_str(input, i, av[j], env)))
			return (char_free_array(av, j));
		j++;
	}
	av[j] = NULL;
	return (av);
}

int		add_redirs(char *input, int *i, t_obj *obj)
{
	while (is_end(input, *i) == 0)
	{
		if (redir_loop(obj, input, i) == -1)
			return (-1);
		if ((is_end(input, *i) == 1))
			break ;
		*i = find_string_end(input, *i);
	}
	return (0);
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
	int		r;
	char	**b_env;
	char	*path;
	int		stock_i;

	path = NULL;
	av = NULL;
	if (redir_loop(obj, input, i) == -1)
		return (-1);
	if (!(obj->obj = sample_str(input, i, obj->obj, env)))
		return (-1);
	stock_i = *i;
	if (add_redirs(input, i, obj) == -1)
		return (-1);
	if (((r = check_path(obj, env, &path)) != 0) || ((r == 0) && !path))
	{
		//ft_printf("after check path\n");
		return (r);
	}
	//ft_printf("check path r[%d] worked for [%s] \n", r, path);
	if (!(b_env = env_to_array(env)))
		return (free_str(path));
	if (!(av = conv_av(input, &stock_i, obj, env)))
		return (free_array_and_str(b_env, -1, path));
	if (try_exec(path, av, b_env, obj) == -1)
		return (free_two_arr_and_str(av, b_env, path, -1));
	return (free_two_arr_and_str(av, b_env, path, 0));
}
