/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/28 11:29:07 by rturcey           #+#    #+#             */
/*   Updated: 2020/04/28 11:32:06 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *origin, size_t size)
{
	free(origin);
	if (!(origin = malloc(size)))
		return (NULL);
	return (origin);
}
