/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:18:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/11 16:58:25 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

char	*handle_86_64(cpu_subtype_t sub)
{
	if ((sub & ~CPU_SUBTYPE_LIB64) == CPU_SUBTYPE_X86_64_H)
		return (ft_strdup("x86_64h"));
	return (ft_strdup("x86_64"));
}

char	*cpu_name(cpu_type_t ct, cpu_subtype_t sub)
{
	if (ct == CPU_TYPE_VAX)
		return (ft_strdup("vax"));
	else if (ct == CPU_TYPE_MC680x0)
		return (ft_strdup("mc680x0"));
	else if (ct == CPU_TYPE_I386)
		return (ft_strdup("i386"));
	else if (ct == CPU_TYPE_X86_64)
		return (handle_86_64(sub));
	else if (ct == CPU_TYPE_MC98000)
		return (ft_strdup("mc98000"));
	else if (ct == CPU_TYPE_HPPA)
		return (ft_strdup("hppa"));
	else if (ct == CPU_TYPE_ARM)
		return (ft_strdup("arm"));
	else if (ct == CPU_TYPE_ARM64)
		return (ft_strdup("arm64"));
	else if (ct == CPU_TYPE_MC88000)
		return (ft_strdup("mc88000"));
	else if (ct == CPU_TYPE_SPARC)
		return (ft_strdup("sparc"));
	else if (ct == CPU_TYPE_I860)
		return (ft_strdup("i860"));
	else if (ct == CPU_TYPE_POWERPC)
		return (ft_strdup("ppc"));
	return (ft_strdup("ppc64"));
}

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
		return (handle_error(file.name, E_NOT_OBJ));
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
		return (handle_error(file.name, E_CORRUPT));
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
		return (handle_error(file.name, E_CORRUPT));
	head->fat = 1;
	little_endian = magic != FAT_MAGIC && magic != FAT_MAGIC_64;
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		res = handle_fat_32(file, head, little_endian);
	return (res);
}
