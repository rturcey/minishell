/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 14:46:39 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/23 10:10:42 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

static void	fill_running(int **new, int i, int pid, t_sh *sh)
{
	new[i][0] = pid;
	new[i][1] = sh->obj->tube[0];
	new[i][2] = sh->obj->tube[1];
	new[i][3] = -1;
	new[i][4] = -1;
	if (sh->obj->prev && sh->obj->prev->pip == IS_PIPE)
	{
		new[i][3] = sh->obj->prev->tube[0];
		new[i][4] = sh->obj->prev->tube[1];
	}
	new[++i] = NULL;
	if (sh->running)
		free(sh->running);
	sh->running = new;
}

static int	add_running(int pid, t_sh *sh)
{
	int **new;
	int count;
	int i;
	int j;

	count = 0;
	while (sh->running && sh->running[count])
		count++;
	if (!(new = malloc(sizeof(int *) * (count + 2))))
		return (-1);
	i = -1;
	while (++i < count)
	{
		if (!(new[i] = malloc(sizeof(int) * 5)))
			return (-1);
		j = -1;
		while (++j < 5)
			new[i][j] = sh->running[i][j];
		free(sh->running[i]);
	}
	if (!(new[i] = malloc(sizeof(int) * 5)))
		return (-1);
	fill_running(new, i, pid, sh);
	return (0);
}

static int	find_running(t_sh *sh, int pid)
{
	int i;
	int j;

	i = -1;
	if (!sh->running)
		return (-1);
	while (sh->running[++i])
	{
		if (sh->running[i][0] != pid)
		{
			j = 0;
			while (++j < 5)
				if (sh->running[i][j] != -1)
					close(sh->running[i][j]);
			kill(sh->running[i][0], SIGINT);
		}
	}
	return (0);
}

static void	close_fds(int lever, t_sh *sh)
{
	if (lever == 1)
	{
		close(sh->obj->tube[1]);
		if (sh->obj->pip != IS_PIPE)
			close(sh->obj->prev->tube[0]);
	}
	if (sh->obj->prev && sh->obj->prev->pip == IS_PIPE)
		close(sh->obj->prev->tube[0]);
	g_forked = 0;
}

void		handle_parent(pid_t pid, int lever, t_sh *sh)
{
	int		status;

	if (sh->obj->pip == IS_PIPE && (sh->obj->type == 3 || sh->obj->type == 5))
		sh->wait = 1;
	if (sh->wait == 0 && sh->obj->pip != IS_PIPE
	&& sh->obj->prev && sh->obj->prev->pip == IS_PIPE)
	{
		while (wait(&status) > 0)
			find_running(sh, pid);
		if (WIFEXITED(status))
			g_err = WEXITSTATUS(status);
		free_int_arr(sh->running, -1, 0);
	}
	else if (sh->wait == 1 || (sh->obj->pip != IS_PIPE
	&& (!sh->obj->prev || sh->obj->prev->pip != IS_PIPE)))
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_err = WEXITSTATUS(status);
		if (sh->wait == 1 && sh->obj->pip != IS_PIPE)
			sh->wait = 0;
	}
	else if (sh->obj->pip == IS_PIPE)
		add_running(pid, sh);
	close_fds(lever, sh);
}
