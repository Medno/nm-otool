/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:55:30 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/03 17:50:49 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint32_t	convert_little_endian(uint32_t ptr)
{
	return (((unsigned)ptr << 24)
		+ ((unsigned)ptr >> 8 << 24 >> 8)
		+ ((unsigned)ptr >> 16 << 24 >> 16)
		+ ((unsigned)ptr >> 24 << 24 >> 24));
}

uint32_t	to_big_endian(uint8_t l_endian, uint32_t value)
{
	if (l_endian)
		return (convert_little_endian(value));
	return (value);
}
