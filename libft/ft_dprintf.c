/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 14:47:53 by esoulard          #+#    #+#             */
/*   Updated: 2020/08/03 18:51:29 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		go_dprint(int fd, char **stock)
{
	int		i;
	int		len;
	int		ratlen;
	int		stocklen;
	char	*rat;

	rat = NULL;
	len = calculatelen(stock);
	stocklen = 0;
	rat = ft_replace_replaceable(stock[0], &stocklen, 2);
	ratlen = stocklen;
	i = 1;
	while (stock[i])
	{
		rat = ft_printf_strjoin(rat, ft_replace_replaceable(stock[i],
			&stocklen, 2), stocklen, ratlen);
		ratlen += stocklen;
		i++;
	}
	ft_putstr_fd(rat, fd);
	free(stock);
	free(rat);
	return (len);
}

int		ft_dprintf(int fd, const char *arg, ...)
{
	va_list	ap;
	char	**stock;
	char	*cpyarg;
	int		i;

	stock = NULL;
	cpyarg = NULL;
	if (arg[0] == '\0')
		return (0);
	cpyarg = ft_strdup(arg);
	stock = init_tab(cpyarg, stock);
	va_start(ap, arg);
	i = 0;
	if (stock != NULL)
		fill_tab(stock, &cpyarg, ap, i);
	free(cpyarg);
	va_end(ap);
	return (go_dprint(fd, stock));
}
