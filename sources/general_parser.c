/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:30 by rturcey           #+#    #+#             */
/*   Updated: 2020/07/28 20:00:50 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**Haven't used it yet, but will be useful during
**further cmd parsing
*/

int		is_separator(char *str, int i)
{
	if (str[i] == ';' || str[i] == '|')
	{
		if ((i > 0) && (str[i - 1] == '\\'))
			return (0);
		return (1);
	}
	return (0);
}

int		pipeline_end(char *input, int i)
{
	while (input[i] && input[i] != ';')
	{
		if (is_quote(input, i, 0))
		{
			if ((i = get_next_quote(input, i)) == -1)
				return (-1);
		}
		i++;
	}
	if (input[i] && i > 0 && input[i - 1] == '\\')
		pipeline_end(input, i);
	return (i);
}

/*
**Making a clean string, unnecessary quotes removed
*/

void	skim_str(char *sample, int k, int *i)
{
	while (sample[++k] && sample[k + 1])
		sample[k] = sample[k + 1];
	while (sample[k])
		sample[k++] = '\0';
	(*i)++;
}

int		sample_quote_cond(char *input, int *i, char **sample, int *j, t_env *env)
{
	int k;
	int l;
	int check;
	int r;
	int heck;

	k = get_next_quote(*sample, *j) - 1;
	l = (*j) - 1;
	heck = 0;
	if (is_quote(input, *i, '\"') == 1)
	{
		while (++l < k)
		{
			if ((*sample)[l + 1] != '$' && ((*sample)[l] == '\\' && ((*sample)[l + 1] == '\"' ||
				(*sample)[l + 1] == '\\') && k--))
				skim_str(*sample, l - 1, i);
			else if ((*sample)[l] == '$' && l > 0 && (*sample)[l - 1] == '\\' && k--)
				skim_str(*sample, l - 2, i);
			else if ((*sample)[l] == '$' && normed_char((*sample)[l + 1]) == 0)
			{
				if ((*sample)[l + 1] && (*sample)[l + 1] == '?')
				{
					if (parse_g_err(sample, &l, i) == -1)
						return (-1);
				}
				else if ((r = parse_sample_var(sample, &l, env, i)) != -3)
				{
					if (r == -1)
						return (-1);
					heck = 1;
					if (r == -2)
						(*j)--;
				}
				k = get_next_quote(*sample, *j) - 1;
			}
		}
	}
	l = k - 1;
	check = -1;
	while (++check != 2)
	{
		skim_str(*sample, k, i);
		k = (*j) - 1;
	}
	if (heck == 0)
		(*i) += l - (*j);
	(*j) = l;
	return (0);
}

char	*sample_str(char *input, int *i, char *sample, t_env *env)
{
	int end;
	int j;
	int r;

	if (!input[*i])
		return (NULL);
	end = find_string_end(input, *i);
	if (!(sample = ft_substr(input, *i, (end - *i))))
		return (NULL);
	j = -1;
	while (sample[++j])
	{
		if (sample[j] == '\\' && sample[j + 1] != '$')
			skim_str(sample, j - 1, i);
		else if (is_quote(input, *i, 0) == 1)
		{
			if (sample_quote_cond(input, i, &sample, &j, env) == -1)
				return (char_free_str(sample));
		}
		else if ((sample[j] == '$') && sample[j + 1] && (sample[j + 1] == '?') \
		&& (j == 0 || sample[j - 1] != '\\'))
		{
			if (parse_g_err(&sample, &j, i) == -1)
				return (char_free_str(sample));
		}
		else if (sample[j] == '$' && j > 0 && sample[j - 1] == '\\')
			skim_str(sample, j - 2, i);
		else if (sample[j] == '$' && normed_char(sample[j + 1]) == 0)
		{
			if ((r = parse_sample_var(&sample, &j, env, i)) == -2)
				j--;
			else if (r == -1)
				return (char_free_str(sample));
		}
		(*i)++;
	}
	*i = end;
	return (sample);
}

/*
**First check for lonely quote, if so return error
**enter loop (after passing first spaces), make a
**sample from the first string we can extract from
**input. It is already parsed and clean.
**We strncmp it with the cmds, init an object if
**strcmp is positive, and send obj and input to
**the appropriate function
*/

void	init_pipe(char *input, int i)
{
	int	end;

	if (input[i])
	{
		end = find_end(input, i);
		if ((--end >= 0 && input[end] == '|'))
			g_p.lever = 1;
		else
		{
			g_p.lever = 0;
			if (g_p.count > 0)
				g_p.type = 3;
		}
	}
	else
	{
		g_p.lever = 0;
		if (g_p.count > 0)
			g_p.type = 3;	
	}
}

void	pipe_checks(char *input, int *i, t_obj *obj)
{
	int status;

	if (g_p.lever == 1) // ROUND 2 : WE HAVE A PIPE BEFORE CMD
	{
		g_p.count++;// ADDING NEXT FORK
		if ((g_p.pid = fork()) == -1) // START CHILD PROCESS
		{
			ft_dprintf(2, "fork error\n");
			exit(EXIT_FAILURE);
		}
		if (g_p.pid == 0) // CHILD PROCESS
		{
			obj->redir->cmd_output = 1; // REVERTING TO REGULAR EXIT INSTEAD OF G_P.PIPEFD[1]
			close(g_p.pipefd[1]); // CLOSE UNUSED WRITE END
			dup2(g_p.pipefd[0], 0); // DUP PIPE INPUT TO 0
			g_p.forked = 2; // JUST TO KNOW THIS PROCESS IS FORKED, DUNNO IF I NEED IT OR NOT IN THE END I AM JUST CONFUSED MAN
			g_p.type = 1; // TO KNOW WE'RE DEALING WITH A CHILD
			ft_dprintf(2, "CHILD after fork %d\n", g_p.count);
		}
		else // PARENT PROCESS
		{
			ft_dprintf(2, "CHILD NOW PARENT after fork waiting\n");
			close(g_p.pipefd[0]); // CLOSE UNUSED READ END
			dup2(g_p.pipefd[1], 1); // DUP PIPE OUTPUT TO 1
			status = 0;
			wait(&status); // WAIT FOR CHILD EXIT SIGNAL
			close(g_p.pipefd[1]); // ONCE CHILD IS DEAD, CLOSING PIPE OUTPUT
			g_p.count--; // ONE LESS FORK!
			ft_dprintf(2, "CHILD/PARENT %d right before exit\n", g_p.count);
			exit(EXIT_SUCCESS);
		}
	}
	init_pipe(input, *i); // ROUND ONE, CHECK IF PIPE AFTER CMD AND INIT VALUES
	if (g_p.lever == 1) // PIPE FOUND AFTER CMD
	{
		if (g_p.count == 0)// IF FIRST CMD OF PIPELINE, WE NEED A FIRST FORK
		{
			g_p.count++; // ADDING NEXT FORK
			if ((g_p.pid = fork()) == -1) // START 1ST CHILD PROCESS
			{
				ft_dprintf(2, "fork error\n");
				exit(EXIT_FAILURE);
			}
			if (g_p.pid != 0)
			{
				wait(&status);// MINISHELL WAITS HERE
				*i = pipeline_end(input, *i);// MINISHELL JUMPS AFTER PIPELINE
				g_p.type = 0;
				return ;
			}
			g_p.type = 2;
		}
		if (pipe(g_p.pipefd) == -1) // OPEN PIPE
		{
			ft_dprintf(2, "pipe error\n");
			exit(EXIT_FAILURE);
		}
		obj->redir->cmd_output = g_p.pipefd[1]; // CAN'T DUP BEFORE FORKING, SO THIS IS COMPULSORY
		ft_dprintf(2, "PIPE CREATED!\n"); // PARENT NOW DOES ITS THING IN GEN PARSER
	}
}

int		general_parser(char *input, t_env *env)
{
	char		*sample;
	int			i;
	t_obj		*obj;
	int			j;
	int			limit = 1;
	int			stock_i;


	if ((lonely_quote(input) == -1) ||
		(last_backslash(input) == -1))
	{
		ft_putstr_fd("bash: multi-line comments not supported\n", 2);
		return (0);
	}
	i = 0;
	i = pass_spaces(input, i);
	while (input[i])
	{
		ft_dprintf(2, "input[%d][%c]\n", i, input[i]);
		if (!(obj = obj_new(env)))
			return (-1);
		if (!(obj->redir = redir_new()))
			return (free_obj(obj));
		sample = NULL;
		//il faudra ajouter un moyen de ne verifier les wrong redir que pour chaque bloc de cmd
		if ((limit-- == 1) && (find_redir_err(obj, input, &i) == -1)
			&& (g_err = 2))
				return (0);
		if ((redir_loop(obj, input, &i) == -1) && (g_err = 2))
			return (-1);
		if (parse_var(input, &i, env, 0) == -1)
		{
			free_obj(obj);
			return (-1);
		}
		pipe_checks(input, &i, obj);
		if (!input[i])
		{
			free_obj(obj);
			continue ;
		}
		stock_i = i;
		if ((sample = sample_str(input, &i, sample, env)) == NULL)
			return (free_obj(obj));
		//ft_printf("string sampled [%s]\n", sample);
		if ((j = is_cmd(sample)) != -1)
		{
			init_obj(obj, sample, j);
			if (obj->obj == NULL)
			{
				free_obj(obj);
				return (-1);
			}
			if ((j = parse_cmds(obj, input, &i, env)) == -1)
			{
				free_obj(obj);
				free_str(sample);
				return (0);
			}
			if (set_g_err(obj, sample) == 1)
				return (-1);
			free(sample);
		}
		else
		{
			i = stock_i;
			free(sample);
			if ((j = parse_exec(obj, input, &i, env)) == -1)
				return (free_obj(obj));
			else if (j == -2)
			{
				maj_err(obj, ft_sprintf("%s: command not found\n", obj->obj), 127);
				print_result(obj, 0, NULL);
				// free_obj(obj);
				// return (0);
			}
		}
		i = find_end(input, i);

		if (obj)
			free_obj(obj);
		if (g_p.type == 3)// is the last cmd in the pipeline
		{
			close(g_p.pipefd[0]);
			ft_dprintf(2, "LAST CHILD %d right before exit\n", g_p.count);
			exit(EXIT_SUCCESS);
		}
		ft_dprintf(2, "END input[%d][%c]\n", i, input[i]);
	}
	return (0);
}
