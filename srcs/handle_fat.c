/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:18:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 13:37:39 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	contain_arch(t_fhead *h, t_fa **o_fa, uint32_t n_fa, uint8_t l_endian)
{
	t_fa		*fa;
	uint32_t	cputype;
	uint32_t	cpusubtype;

	fa = (t_fa *)(h->ptr + sizeof(t_fh));
	while (n_fa)
	{
		cputype = to_big_endian(l_endian, fa->cputype);
		cpusubtype = to_big_endian(l_endian, fa->cpusubtype);
		if ((CPU_TYPE_X86 | CPU_ARCH_ABI64) == cputype
		&& (cpusubtype & ~CPU_SUBTYPE_LIB64) == (CPU_SUBTYPE_X86_64_ALL))
		{
			*o_fa = fa;
			return (1);
		}
		fa = (void *)fa + sizeof(t_fa);
		n_fa--;
	}
	return (0);
}

uint8_t	handle_fat_arch(t_finfo file, t_fhead *head, t_fa *fa, uint8_t l_end)
{
	uint8_t	res;

	if (invalid_filetype(head->ptr + to_big_endian(l_end, fa->offset)))
	{
		free(head->fat_arch);
		return (handle_error(file.name, E_NOT_OBJ, file.opts));
	}
	head->current = head->ptr + to_big_endian(l_end, fa->offset);
	if (is_archive(head->ptr + to_big_endian(l_end, fa->offset)))
		res = handle_archive(file, head);
	else
		res = list_symbols(file, head, file.name);
	if (res)
		free(head->fat_arch);
	return (res);
}

uint8_t	handle_fat_32(t_finfo file, t_fhead *head, uint8_t l_endian)
{
	uint32_t	n_fa;
	t_fh		*fh;
	t_fa		*fa;

	fh = (t_fh *)head->ptr;
	n_fa = to_big_endian(l_endian, fh->nfat_arch);
	if (head->ptr + sizeof(*fh) + (sizeof(*fa) * n_fa) > head->ptr + file.size)
		return (handle_error(file.name, E_CORRUPT, file.opts));
	fa = (t_fa *)(head->ptr + sizeof(t_fh));
	if (contain_arch(head, &fa, n_fa, l_endian))
		return (handle_fat_arch(file, head, fa, l_endian));
	while (n_fa)
	{
		if ((char *)fa + to_big_endian(l_endian, fa->size)
				> head->ptr + file.size)
			return (1);
		head->fat_arch = cpu_name(to_big_endian(l_endian,
			fa->cputype), to_big_endian(l_endian, fa->cpusubtype));
		if (handle_fat_arch(file, head, fa, l_endian))
			return (1);
		fa = (void *)fa + sizeof(t_fa);
		n_fa--;
		free(head->fat_arch);
	}
	return (0);
}

uint8_t	handle_fat(t_finfo file, t_fhead *head, uint32_t magic)
{
	uint8_t	little_endian;
	uint8_t	res;

	res = 1;
	if (head->ptr + sizeof(struct fat_header) > head->ptr + file.size)
		return (handle_error(file.name, E_CORRUPT, file.opts));
	head->fat = 1;
	little_endian = magic != FAT_MAGIC && magic != FAT_MAGIC_64;
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		res = handle_fat_32(file, head, little_endian);
	return (res);
}
