/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/10 11:24:10 by rturcey           #+#    #+#             */
/*   Updated: 2020/10/14 09:22:35 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		count_words(char *s, char c)
{
	int i;
	int count;

	count = 0;
	i = -1;
	while (s[0] != '\0' && s[++i])
	{
		split_pass_quotes(s, &i);
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			count++;
		if (!s[i])
			break ;
	}
	if (s[0] != c)
		count++;
	return (count);
}

static char		**finish_him(char **result, int j, int word_count)
{
	if (j == word_count + 1)
		result[j] = NULL;
	else
	{
		free(result[j]);
		result[j] = NULL;
	}
	return (result);
}

static char		*newline(char **result, t_sh *sh, int *j, int *i)
{
	if (!(result[*j] = sample_str(sh, i, result[*j])))
		return (NULL);
	return (result[(*j)++]);
}

char			**ft_split_var(char *s, char c, t_sh *sh, int *i)
{
	char	**result;
	int		j;

	j = 0;
	if (!s || !(result = malloc(sizeof(char *) * (count_words(s, c) + 2))))
		return (NULL);
	while (is_end(s, *i) == 0)
	{
		while ((is_end(s, *i) == 0) && s[*i] == c)
			(*i)++;
		if ((is_end(s, *i) == 0))
			if (newline(result, sh, &j, i) == NULL)
				return (char_free_array(result, -1));
	}
	return (finish_him(result, j, (count_words(s, c) - 1)));
}

int				is_quote_mod(char *str, int i, char quote, int l)
{
	int count;

	count = 0;
	if (quote == 0)
	{
		if (str[i] && (str[i] == '\'' || str[i] == '\"'))
		{
			if ((i > l) && (str[i - 1] == '\\'))
			{
				if (str[i] == '\'' && str[i - 1])
					if (check_single(str, i) == 1)
						return (1);
				while (--i > l && str[i] && str[i] == '\\')
					count++;
				if (count % 2 != 0)
					return (0);
			}
			return (1);
		}
	}
	return (0);
}
