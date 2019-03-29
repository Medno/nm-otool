/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_misc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 11:16:35 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 11:23:25 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_MISC_H
# define LIB_MISC_H

# include "libft.h"

# define BUFF_SIZE 1000

typedef struct	s_gnl
{
	int		fd;
	char	value[BUFF_SIZE + 1];
}				t_gnl;

int				get_next_line(const int fd, char **line);

char			*ft_getopt(char **cmd, int *i);
char			*ft_itoa(int n);

char			ft_lastchar(char *str);

int				ft_atoi(const char *str);
int				ft_checkopt(char *p, char *str, int nb_opt);
int				ft_nbchar(char *str, char c);
int				ft_nbwords(char *str);
int				ft_tolower(int c);
int				ft_toupper(int c);

void			ft_bzero(void *s, size_t n);
void			ft_foreachint(int *tab, int length, void (*f)(int));

char			**ft_tabdup(char **tab);
void			ft_tabdel(char **tab);
void			ft_del_array(void **array);

#endif
