/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/06 10:18:21 by rturcey           #+#    #+#             */
/*   Updated: 2020/05/06 13:10:44 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		free_array(char **array, int max)
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
	return (-1);
}

int		free_str(char *str)
{
	free(str);
	str = NULL;
	return (-1);
}

int		free_two_str(char *s1, char *s2)
{
	free_str(s1);
	free_str(s2);
	return (-1);
}

int		free_array_and_str(char **arr, char *str)
{
	free_array(arr, -1);
	free_str(str);
	return (-1);
}
