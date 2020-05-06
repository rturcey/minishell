/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 10:18:21 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/06 10:30:33 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*free_array(char **array, int max)
{
	int i;
	if (max == -1)
	{
		max = 0;
		while (array[max] != NULL)
			max++;
	}
	i = -1;
	while (++i < max)
		free(array[i]);
	free(array);
	return (NULL);
}

int		free_two_str(char *s1, char *s2)
{
	free(s1);
	free(s2);
	return (-1);
}

int		free_array_and_str(char **arr, char *str)
{
	free_array(arr, -1);
	free(str);
	return (-1);
}
