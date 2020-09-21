/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/03 16:59:02 by rturcey           #+#    #+#             */
/*   Updated: 2020/09/21 20:13:26 by esoulard         ###   ########.fr       */
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

# define YELLOW 	"\033[1;93m"
# define CYAN 		"\033[1;96m"
# define END  		"\033[0m"
# define IS_PIPE	1


typedef struct stat	t_stat;

typedef struct		s_env
{
	char			*key;
	char			*val;
	int				in;
	struct s_env	*next;
	int				pluseq;
}					t_env;

typedef struct		s_redir
{
	int				cmd_in;
	int				cmd_output;
	int				err_output;
}					t_redir;

typedef struct		s_pipe
{
	int				lever;
	int				pipefd[2];
	int				pid;
	int				forked;
	int				type;
	int				count;
}					t_pipe;

typedef struct		s_obj
{
	char			*obj;
	int				type;
	int				option;
	t_redir			*redir;
	char			*result;
	char			*error;
	int				pip;
	char			**args;
	t_env			*env;
	char			**charenv;
	int				tube[2];
	int				lev;
	int wait;
	struct s_obj	*next;
	struct s_obj	*prev;
}					t_obj;

typedef struct		s_sh
{
	t_obj			*obj;
	t_env			*env;
	char			*in;
	int				lev;
	int 			**running;
	int 			wait;
	int				*tmp;
	int				*temp;
}					t_sh;

t_env				*g_lstenv;
int					g_forked;
int					g_err;

typedef int			(*t_parse_cmd)(t_sh *, int *);

int					is_ms(char *exec);
void				set_gfork(char *tmp);
void				sighandler(int num);
void				sighandler_exec(int num);
int					general_parser(t_sh *sh);
void				remove_home_path(char **path, char *home);
int					skim_str(char *sample, int k, int *i);
t_env				*init_env(char **env, int in);
int					env_clear(t_env *env);
void				print_env(t_env *env, int fd);
t_env				*env_new(int in);
char				*find_env_val(char *key, t_env *env);
t_env				*find_env_entry(char *key, t_env *env);
t_env				*env_cpy(t_env *elt);
char				**env_to_array(t_env *env);
int					export_var(t_env *elt, t_env *env);
void				del_var(t_env *var);
void				del_from_key(t_env **begin, char *key);
t_obj				*obj_new(t_env *env, t_obj *prev);
t_redir				*redir_new(void);
char				*init_obj(t_obj *obj, char *sample, int type);
int					free_obj(t_obj **obj);
void				*free_redir(t_redir *redir);
int					parse_exec(t_sh *sh, int *i);
int					is_cmd(char *sample);
int					parse_cmds(t_sh *sh, int *i);
int					parse_echo(t_sh *sh, int *i);
int					parse_cd(t_sh *sh, int *i);
int					parse_pwd(t_sh *sh, int *i);
int					parse_export(t_sh *sh, int *i);
int					parse_unset(t_sh *sh, int *i);
int					parse_env(t_sh *sh, int *i);
int					parse_exit(t_sh *sh, int *i);
int					pass_spaces(char *str, int i);
int					is_quote(char *str, int i, char quote);
int					lonely_quote(char *str);
int					last_backslash(char *str);
int					find_string_end(char *src, int i);
int					find_end(char *in, int i);
int					find_redir(t_sh *sh, int *i);
int					find_redir_err(t_sh *sh, int *i);
int					is_redir(char *str, int i);
char				*sample_str(t_sh *sh, int *i, char *sample);
int					is_separator(char *str, int i);
int					is_end(char *str, int i);
int					get_next_quote(char *str, int i);
int					count_strings(char *in, int i);
int					unset_var(char **elt, t_env *env);
int					add_var(t_env *elt, t_env *env);
int					parse_var(t_sh *sh, int *i, int len);
int					print_result(t_sh *sh, int ret, char *to_free);
void				maj_err(t_sh *sh, char *str, int err);
int					split_env(char *line, t_env *elt);
int					sample_export(char *sample, t_env *env);
int					check_var(char *sample);
int					parse_sample_var(char **sample, int *j, t_env *env, int *i);
int					set_g_err(t_sh *sh);
int					parse_g_err(char **sample, int *l, int *i);
int					redir_loop(t_sh *sh, int *i);
int					replace_pwd(t_env *env, char **path);
int					check_path(t_sh *sh, char **path);
void				pass_option(t_sh *sh, int *i);
int					normed_char(char c);
int					pluseq(char *sample, int i);
int					repluseq(char *sample, int i, t_env *env, int in);
int					parse_syntax(t_sh *sh, int i);
void				export_solo(t_sh *sh);
void				clear_sh(t_sh *sh);
int					init_main(t_sh **sh, char **env);
void				pipe_checks(t_sh *sh, int *i);
void				parent_handling(t_sh *sh);
void				start_exec(char *tmp, char **av, char **env, t_sh *sh);
void				handle_parent(pid_t pid, int lever, t_sh *sh);
int	try_exec(char *tmp, t_sh *sh, int *i);

#endif
