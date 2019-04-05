/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memalloc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/08 16:39:25 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/05 15:37:17 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memalloc(size_t size)
{
	char	*tmp;
	size_t	i;

	i = 0;
	if (!size)
		return (NULL);
	if (!(tmp = (char *)malloc(size)))
		return (NULL);
	while (i < size)
		tmp[i++] = 0;
	return (tmp);
}

void	ft_memdel(void **ap)
{
	if (ap)
	{
		free(*ap);
		*ap = NULL;
	}
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		tmp[len];
	const char	*tmp_src;
	char		*tmp_dst;
	size_t		i;

	i = 0;
	tmp_dst = dst;
	tmp_src = src;
	if (len > 100000000)
		return (0);
	while (i < len)
	{
		tmp[i] = tmp_src[i];
		i++;
	}
	i = 0;
	while (i < len)
	{
		tmp_dst[i] = tmp[i];
		i++;
	}
	return (tmp_dst);
}

void	*ft_memalloc_uint(size_t size)
{
	uint32_t	*tmp;
	size_t		i;

	i = 0;
	if (!size)
		return (NULL);
	if (!(tmp = (uint32_t *)malloc(sizeof(uint32_t) * (size + 1))))
		return (NULL);
	while (i < size)
		tmp[i++] = 0;
	tmp[i] = '\0';
	return (tmp);
}
