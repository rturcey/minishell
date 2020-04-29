/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:28:57 by rturcey           #+#    #+#             */
/*   Updated: 2020/04/29 16:41:51 by rturcey          ###   ########.fr       */
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

# define GREEN "\033[1m\033[32m"
# define BLUE "\033[1m\033[34m"
# define END  "\033[0m"

typedef struct		s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

int					general_parser(char *input);
t_env				*init_env(char **env);
void				*env_clear(t_env *env);
void				print_env(t_env *env);
t_env				*env_new(void);
char				*find_env_value(char *key, t_env *env);
t_env				*find_env_entry(char *key, t_env *env);
t_env				*env_cpy(t_env *elt);
int					export_var(t_env *elt, t_env *env);

#endif
