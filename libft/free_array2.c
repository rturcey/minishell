/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rturcey <rturcey@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 22:03:52 by esoulard          #+#    #+#             */
/*   Updated: 2020/09/23 10:05:49 by rturcey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		free_two_arrays(char **arra, char **arrb)
{
	free_array(arra, -1, -1);
	free_array(arrb, -1, -1);
	return (-1);
}

int		free_two_arr_and_str(char **arra, char **arrb, char *str, int ret)
{
	free_array(arra, -1, -1);
	free_array(arrb, -1, -1);
	free_str(str);
	return (ret);
}

int		free_two_arr_two_str(char **arra, char **arrb, char *stra, char *strb)
{
	free_array(arra, -1, -1);
	free_array(arrb, -1, -1);
	free_str(stra);
	free_str(strb);
	return (-1);
}

char	**char_free_array(char **arr, int max)
{
	free_array(arr, max, -1);
	return (NULL);
}

int		free_int_arr(int **array, int max, int ret)
{
	int i;

	if (!array)
		return (-1);
	if (max == -1)
	{
		max = 0;
		while (array[max] != NULL)
			max++;
	}
	i = -1;
	while (++i < max)
	{
		free(array[i]);
		array[i] = NULL;
	}
	free(array);
	array = NULL;
	return (ret);
}
