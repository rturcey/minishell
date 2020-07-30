/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 14:06:46 by esoulard          #+#    #+#             */
/*   Updated: 2020/07/30 10:10:34 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** So far, the program launches but minishell crashes when it shuts.
** I need to look into forking and processes and stuff.
*/

int		try_exec(char *tmp, char **av, char **env, t_sh *sh)
{
	pid_t	pid;
	int		status;

	pid = -1;
	if ((pid = fork()) < 0)
	{
		ft_dprintf(2, ft_sprintf("fork: %s\n", strerror(errno)));
		return (-1);
	}
	else if (pid == 0)
	{
		if ((dup2(sh->obj->redir->cmd_output, 1) == -1) ||
			dup2(sh->obj->redir->err_output, 2) == -1)
			return (-1);
		if (sh->obj->redir->cmd_in >= 0)
			if (dup2(sh->obj->redir->cmd_in, 0) == -1)
				return (-1);
		execve(tmp, av, env);
	}
	else if ((status = 0) == 0)
	{
		wait(&status);
		sh->err = status;
	}
	return (0);
}

/*
** Putting potential params for the program in a char **av
*/

char	**conv_av(t_sh *sh, int *i)
{
	int		j;
	char	**av;

	if (!(av = malloc(sizeof(char *) * (count_strings(sh->in, *i) + 2))))
		return (NULL);
	j = 0;
	if (!(av[j++] = ft_strdup(sh->obj->obj)))
		return (char_free_array(av, 0));
	while (is_end(sh->in, *i) == 0)
	{
		if (redir_loop(sh, i) == -1)
			return (char_free_array(av, j));
		if ((is_end(sh->in, *i) == 1))
			break ;
		if (!(av[j] = sample_str(sh, i, av[j])))
			return (char_free_array(av, j));
		j++;
	}
	av[j] = NULL;
	return (av);
}

int		add_redirs(t_sh *sh, int *i)
{
	while (is_end(sh->in, *i) == 0)
	{
		if (redir_loop(sh, i) == -1)
			return (-1);
		if ((is_end(sh->in, *i) == 1))
			break ;
		*i = find_string_end(sh->in, *i);
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

int		parse_exec(t_sh *sh, int *i)
{
	char	**av;
	int		r;
	char	**b_env;
	char	*path;
	int		stock_i;

	path = NULL;
	av = NULL;
	if (redir_loop(sh, i) == -1)
		return (-1);
	if (!(sh->obj->obj = sample_str(sh, i, sh->obj->obj)))
		return (-1);
	stock_i = *i;
	if (add_redirs(sh, i) == -1)
		return (-1);
	if (((r = check_path(sh, &path)) != 0) || ((r == 0) && !path))
		return (r);
	if (!(b_env = env_to_array(sh->env)))
		return (free_str(path));
	if (!(av = conv_av(sh, &stock_i)))
		return (free_array_and_str(b_env, -1, path));
	if (try_exec(path, av, b_env, sh) == -1)
		return (free_two_arr_and_str(av, b_env, path, -1));
	return (free_two_arr_and_str(av, b_env, path, 0));
}
