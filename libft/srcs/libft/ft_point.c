/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_point.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/23 10:59:23 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 10:49:19 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_point		set_point(int x, int y)
{
	t_point result;

	result.x = x;
	result.y = y;
	return (result);
}

t_point_32	set_point_32(uint32_t x, uint32_t y)
{
	t_point_32	res;

	res.x = x;
	res.y = y;
	return (res);
}
