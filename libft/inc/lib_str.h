/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_str.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 10:57:43 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 11:15:24 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_STR_H
# define LIB_STR_H

# include "libft.h"

char			**ft_strsplit(char const *s, char c);
char			**ft_splitwsp(char *str);

char			*ft_strcat(char *s1, const char *s2);
char			*ft_strncat(char *s1, const char *s2, size_t n);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strlchr(const char *s, int c);
char			*ft_strfchr(char *str, char c);
char			*ft_strcpy(char *dst, const char *src);
char			*ft_strncpy(char *dst, const char *src, size_t len);
char			*ft_strdup(const char *s1);
char			*ft_strndup(const char *s1, size_t len);
char			*ft_strgap(char *str, char delim, int pos);
char			*ft_strlgap(char *str, char delim, int pos);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strjoindel(char *s1, char *s2);
char			*ft_strmap(char const *s, char (*f)(char));
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char			*ft_strnew(size_t size);
char			*ft_strstr(const char *haystack, const char *needle);
char			*ft_strnstr(const char *haystack, const char *needle, size_t n);
char			*ft_strsub(char const *s, unsigned int start, size_t len);
char			*ft_strtrim(char const *s);
char			*ft_stolower(char *str);
char			*ft_onejoinf(char *news, char *old);
char			*ft_strdup_count(const char *s1, const char searched, int *c);
char			*ft_put_in_str(char *str, char c);

int				ft_strcmp(const char *s1, const char *s2);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strequ(char const *s1, char const *s2);
int				ft_strnequ(char const *s1, char const *s2, size_t n);

size_t			ft_strlcat(char *dst, const char *src, size_t size);
size_t			ft_strlen(const char *s1);

void			ft_strclr(char *s);
void			ft_strdel(char **as);
void			ft_striter(char *s, void (*f)(char *));
void			ft_striteri(char *s, void (*f)(unsigned int, char *));

#endif
