/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 14:55:30 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 17:22:06 by pchadeni         ###   ########.fr       */
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

uint8_t		is_macho(uint32_t magic)
{
	return (magic == MH_MAGIC || magic == MH_MAGIC_64
			|| magic == MH_CIGAM || magic == MH_CIGAM_64);
}

uint8_t		is_fat(uint32_t magic)
{
	return (magic == FAT_MAGIC || magic == FAT_MAGIC_64
			|| magic == FAT_CIGAM || magic == FAT_CIGAM_64);
}

uint8_t		is_archive(char *ptr)
{
	char	tmp[SARMAG];

	ft_strncpy(tmp, ptr, SARMAG);
	if (ft_strnequ(tmp, ARMAG, SARMAG))
		return (1);
	return (0);
}

uint32_t	to_big_endian(uint8_t l_endian, uint32_t value)
{
	if (l_endian)
		return (convert_little_endian(value));
	return (value);
}
