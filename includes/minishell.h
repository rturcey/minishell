/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:02 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/14 10:17:44 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "libft.h"

# define YELLOW "\033[1;93m"
# define CYAN "\033[1;96m"
# define END  "\033[0m"

typedef struct		struct_stat t_stat;

typedef struct		s_env
{
	char			*key;
	char			*value;
	int				in;
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
	int				err_output;
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
	char			*error;
	int				ret;
	t_env			*env;
	struct s_obj	*next;
}					t_obj;

int					g_err;

typedef int			(*t_parse_cmd)(t_obj *, char *, int *, t_env *);

int					general_parser(char *input, t_env *env);
void				skim_str(char *sample, int k, int *i);
t_env				*init_env(char **env, int in);
int					env_clear(t_env *env);
void				print_env(t_env *env, int fd);
t_env				*env_new(int in);
char				*find_env_value(char *key, t_env *env);
t_env				*find_env_entry(char *key, t_env *env);
t_env				*env_cpy(t_env *elt);
char				**env_to_array(t_env *env);
int					export_var(t_env *elt, t_env *env);
void				del_var(t_env *var);
void				del_from_key(t_env **begin, char *key);
t_obj				*obj_new(t_env *env);
t_redir				*redir_new(void);
void				init_obj(t_obj *obj, char *sample, int type);
int					free_obj(t_obj *obj);
void				*free_redir(t_redir *redir);
int					parse_exec(t_obj *obj, char *input, int *i, t_env *env);
int					is_cmd(char *sample);
int					parse_cmds(t_obj *obj, char *input, int *i, t_env *env);
int					parse_echo(t_obj *obj, char *input, int *i, t_env *env);
int					parse_cd(t_obj *obj, char *input, int *i, t_env *env);
int					parse_pwd(t_obj *obj, char *input, int *i, t_env *env);
int					parse_export(t_obj *obj, char *input, int *i, t_env *env);
int					parse_unset(t_obj *obj, char *input, int *i, t_env *env);
int					parse_env(t_obj *obj, char *input, int *i, t_env *env);
int					parse_exit(t_obj *obj, char *input, int *i, t_env *env);
int					pass_spaces(char *str, int i);
int					is_quote(char *str, int i, char quote);
int					lonely_quote(char *str);
int					last_backslash(char *str);
int					find_string_end(char *src, int i);
int					find_redir(t_obj *obj, char *input, int *i);
int					find_redir_err(t_obj *obj, char *input, int *i);
int					is_redir(char *str, int i);
char				*sample_str(char *input, int *i, char *sample, t_env *env);
int					is_separator(char *str, int i);
int					is_end(char *str, int i);
int					get_next_quote(char *str, int i);
int					count_strings(char *input, int i);
int					unset_var(char **elt, t_env *env);
int					add_var(t_env *elt, t_env *env);
int					parse_var(char *input, int *i, t_env *env, int len);
int					print_result(t_obj *obj, int ret, char *to_free);
void				maj_err(t_obj *obj, char *str, int err);
int					split_env(char *line, t_env *elt);
int					sample_export(char *sample, t_env *env);
int					check_var(char *sample);
int					parse_sample_var(char **sample, int *j, t_env *env, int *i);
int					set_g_err(t_obj *obj, char *sample);
int					parse_g_err(char **sample, int *l, int *i);
int					redir_loop(t_obj *obj, char *input, int *i);
int					replace_pwd(t_env *env);
int					check_path(t_obj *obj, char *sample, t_env *env, char **path);

#endif
