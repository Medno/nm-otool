/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:18:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/05 18:05:43 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

char	*cpu_name(cpu_type_t ct)
{
	if (ct == CPU_TYPE_VAX)
		return (ft_strdup("vax"));
	else if (ct == CPU_TYPE_MC680x0)
		return (ft_strdup("mc680x0"));
	else if (ct == CPU_TYPE_I386)
		return (ft_strdup("i386"));
	else if (ct == CPU_TYPE_X86_64)
		return (ft_strdup("x86_64"));
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

uint8_t	contain_architecture(const NXArchInfo *arch, t_fa *fa, uint32_t n_fa, uint8_t l_endian)
{
	while (n_fa)
	{
		if (arch->cputype == (cpu_type_t)to_big_endian(l_endian, fa->cputype))
			return (1);
		fa = (void *)fa + sizeof(t_fa);
		n_fa--;
	}
	return (0);
}

uint8_t	handle_fat_arch(char *arg, char *ptr, t_fa *fa, uint8_t l_endian)
{
	uint8_t	res;

	if (invalid_filetype(ptr + to_big_endian(l_endian, fa->offset)))
		return (1);
	if (is_archive(ptr + to_big_endian(l_endian, fa->offset)))
		res = handle_archive(arg, ptr + to_big_endian(l_endian, fa->offset));
	else
		res = list_symbols(arg, ptr + to_big_endian(l_endian, fa->offset));
	return (res);
}

uint8_t	handle_fat_32(char *arg, char *ptr, uint8_t l_endian, const NXArchInfo *arch)
{
	uint32_t	n_fa;
	t_fh		*fh;
	t_fa		*fa;
	uint8_t		res;
	char		*cpu_type;

	fh = (t_fh *)ptr;
	n_fa = to_big_endian(l_endian, fh->nfat_arch);
	fa = (t_fa *)(ptr + sizeof(t_fh));
	if (contain_architecture(arch, fa, n_fa, l_endian))
		return (handle_fat_arch(arg, ptr, fa, l_endian));
	while (n_fa)
	{
		cpu_type = cpu_name(to_big_endian(l_endian, fa->cputype));
		ft_printf("\n%s (for architecture %s):\n", arg, cpu_type);
		free(cpu_type);
		res = handle_fat_arch(arg, ptr, fa, l_endian);
		fa = (void *)fa + sizeof(t_fa);
		n_fa--;
	}
	return (0);
}

uint8_t	handle_fat(char *arg, char *ptr, uint32_t magic)
{
	uint8_t				little_endian;
	uint8_t				res;
	const NXArchInfo	*arch;

	arch = NXGetLocalArchInfo();
	res = 1;
	little_endian = magic != FAT_MAGIC && magic != FAT_MAGIC_64;
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		res = handle_fat_32(arg, ptr, little_endian, arch);
	return (res);
}
