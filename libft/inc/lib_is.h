/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_is.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 10:56:01 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 11:25:12 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_IS_H
# define LIB_IS_H

# include "libft.h"

typedef struct stat	t_stat;

int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);

uint8_t	ft_iswsp(char c);
uint8_t	ft_isdir(t_stat sb);
uint8_t	ft_isdot(char *path);
uint8_t	ft_isexe(t_stat stat);
uint8_t	ft_islink(t_stat stat);

#endif
