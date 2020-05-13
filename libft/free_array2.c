/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/12 22:03:52 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/13 14:35:56 by esoulard         ###   ########.fr       */
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
	return (0);
}
