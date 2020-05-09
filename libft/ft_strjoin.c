/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 20:45:47 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/09 11:11:11 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*ft_printf_strjoin(char *s1, char *s2, int stocklen, int ratlen)
{
	char	*new;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!(new = ft_malloc("char", (ratlen + stocklen + 1))))
		return (NULL);
	while (i < ratlen)
	{
		new[i] = s1[i];
		i++;
	}
	while (j < stocklen)
		new[i++] = s2[j++];
	new[i] = '\0';
	ft_free(s1);
	ft_free(s2);
	return (new);
}

char	*ft_strjoin_sp(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = 0;
	new = NULL;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	while (s2[j])
		j++;
	if (!(new = malloc(sizeof(char) * (ft_pstrlen(s1) + j + 2))))
		return (NULL);
	while (s1[++i])
		new[i] = s1[i];
	j = 0;
	if (ft_pstrlen(s1) > 0)
		new[i++] = ' ';
	while (s2[j])
		new[i++] = s2[j++];
	new[i] = '\0';
	free(s1);
	s1 = NULL;
	free(s2);
	s2 = NULL;
	return (new);
}

char	*ft_strjoin_bth(char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	i = -1;
	j = 0;
	new = NULL;
	if (s1 == NULL || s2 == NULL)
		return (NULL);
	while (s2[j])
		j++;
	if (!(new = malloc(sizeof(char) * (ft_pstrlen(s1) + j + 1))))
		return (NULL);
	while (s1[++i])
		new[i] = s1[i];
	j = 0;
	while (s2[j])
		new[i++] = s2[j++];
	new[i] = '\0';
	free(s1);
	s1 = NULL;
	free(s2);
	s2 = NULL;
	return (new);
}
