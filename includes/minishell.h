/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:57 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/01 03:17:14 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "libft.h"

# define YELLOW "\033[1;93m"
# define CYAN "\033[1;96m"
# define END  "\033[0m"

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct		s_arg
{
	char			*value;
	struct s_arg	*next;
}					t_arg;

typedef struct		s_redir
{
	int				cmd_input;
	int				cmd_output;
	char			*cmd_output_pos;
	int				err_output;
	char			*err_output_pos;
}					t_redir;

typedef struct		s_obj
{
	char			*obj;
	int				type;
	int				option;
	int				args_count;
	t_arg			*args;
	t_redir			*redir;
	char			*result;
	int				ret;
	struct s_obj	*next;
}					t_obj;

typedef int			(*t_parse_cmd)(t_obj *, char *, int *, char *);

int					general_parser(char *input);
t_env				*init_env(char **env);
void				*env_clear(t_env *env);
void				print_env(t_env *env);
t_env				*env_new(void);
char				*find_env_value(char *key, t_env *env);
t_env				*find_env_entry(char *key, t_env *env);
t_env				*env_cpy(t_env *elt);
int					export_var(t_env *elt, t_env *env);
void				del_var(t_env *var);
void				del_from_key(t_env **begin, char *key);
t_obj				*obj_new(void);
t_obj				*init_obj(char *sample, int type);
void				*free_obj(t_obj *obj);
int					is_cmd(char *sample);
int					parse_cmds(t_obj *obj, char *input, int *i, char *sample);
int					parse_echo(t_obj *obj, char *input, int *i, char *sample);
int					parse_cd(t_obj *obj, char *input, int *i, char *sample);
int					parse_pwd(t_obj *obj, char *input, int *i, char *sample);
int					parse_export(t_obj *obj, char *input, int *i, char *sample);
int					parse_unset(t_obj *obj, char *input, int *i, char *sample);
int					parse_env(t_obj *obj, char *input, int *i, char *sample);
int					parse_exit(t_obj *obj, char *input, int *i, char *sample);
int					is_space(char a);
int					pass_spaces(char *str, int i);
int					is_quote(char *str, int i);
int					lonely_quote(char *str);
int					find_string_end(char *src, int i);

#endif
