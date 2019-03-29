/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_maths.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 11:08:02 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 11:23:51 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_MATHS_H
# define LIB_MATHS_H

# include "libft.h"

typedef struct	s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct	s_point_32
{
	uint32_t	x;
	uint32_t	y;
}				t_point_32;

t_point			set_point(int x, int y);
t_point_32		set_point_32(uint32_t x, uint32_t y);

unsigned int	ft_squared_vector_distance(t_point a, t_point b);

int				ft_abs(int n);
int				ft_pow(int n, int pow);
int				ft_sqrt(int nb);

uint8_t			ft_expten(int n);

#endif
