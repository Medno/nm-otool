/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_put.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 10:52:38 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 10:57:37 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_PUT_H
# define LIB_PUT_H

# include "libft.h"

void	ft_putchar(char c);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl(char const *s);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putnbr(int n);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr(char const *s);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putsgap(char *str, char n, int i);
void	ft_puttab(char **tab);
void	ft_putcolor(char *str, char *color);
void	ft_putformat(char *str, char *color, char *background);
void	ft_putnstr(const char *str, size_t len);

#endif
