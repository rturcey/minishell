/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 16:47:31 by esoulard          #+#    #+#             */
/*   Updated: 2020/05/06 15:22:26 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>

char	*g_mallocs[500];
int		g_malnb;
int		g_freenb;

int		ft_printf(const char *arg, ...);
int		ft_dprintf(int fd, const char *arg, ...);
int		ft_sprintf(char **result, const char *arg, ...);

char	*ft_malloc(char *type, int size);
void	ft_free(char *str);

int		eligible_for_empty_str(char *arg, int conv, int precision, char *ret);
int		give_precision_a_damn_len(char *ret);
void	*cpy_arg(const char *arg, char *cpy);
char	**init_tab(char *arg, char **stock);
int		cstrlen(char *arg, int i);
int		actual_len(char *ret);
int		calculatelen(char **str);
char	**fill_tab(char **stock, char **cpyarg, va_list ap, int i);
int		check_width(char *arg, int i);
char	*add_width(char *ret, char *arg, int i, int min_width);
char	*add_options(char *ret, char *arg, int i);
int		check_precision(char *arg, int i);
char	*damn_precision(char *ret, char *arg, int i, int precision);

int		is_type(char *arg, int i);
int		zero_chain(char *ret, char type);
int		check_dash_flag(char *arg, int i);
int		check_zero_filler(char *arg, int i);
int		parse_end_format(char *arg, int i);
int		ft_strncmp(const char *s1, const char *s2, size_t n);

char	*null_str(char *ret);
char	*convert_char(char *ret, va_list ap);
char	*convert_str(char *ret, va_list ap);
char	*convert_addr(char *ret, va_list ap);
char	*convert_signed_int(char *ret, va_list ap);
char	*convert_unsigned_int(char *ret, va_list ap);
char	*convert_unsigned_hexa(char *ret, va_list ap, char d);
char	*convert_percent(char *ret, int *i);

char	*ft_itoa(int n);
int		ft_isdigit(int c);
size_t	ft_pstrlen(const char *s);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strdup(const char *s1);
char	*ft_printf_strjoin(char *s1, char *s2, int stocklen, int ratlen);

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_unsigned_itoa(unsigned int n);
char	*ft_putlnbr_base(unsigned long long lnbr, char d, char *str);
char	*ft_replace_replaceable(char *str, int *stocklen, int i);

int		go_print(char **stock);
int		go_dprint(int fd, char **stock);
int		go_sprint(char **result, char **stock);

void	ft_putstr_fd(char *s, int fd);

#endif
