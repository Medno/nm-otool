/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:18:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 17:11:45 by pchadeni         ###   ########.fr       */
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

uint8_t	handle_fat_32(char *arg, char *ptr, uint8_t l_endian)
{
	uint32_t	n_fa;
	t_fh		*fh;
	t_fa		*fa;
	char		*cpu_type;

	fh = (t_fh *)ptr;
	n_fa = to_big_endian(l_endian, fh->nfat_arch);
	ft_printf("Number of arch : |%zu|\n", n_fa);
	fa = (t_fa *)(ptr + sizeof(t_fh));
	while (n_fa)
	{
		cpu_type = cpu_name(to_big_endian(l_endian, fa->cputype));
		ft_printf("\n%s (for architecture %s):\n", arg, cpu_type);
		free(cpu_type);
		if (invalid_filetype(ptr + to_big_endian(l_endian, fa->offset)))
			return (1);
		if (is_archive(ptr + to_big_endian(l_endian, fa->offset)))
			handle_archive(arg, ptr + to_big_endian(l_endian, fa->offset));
		else
			list_symbols(arg, ptr + to_big_endian(l_endian, fa->offset));
		fa = (void *)fa + sizeof(t_fa);
		n_fa--;
	}
	return (0);
}

uint8_t	handle_fat(char *arg, char *ptr, uint32_t magic)
{
	uint8_t	little_endian;
	uint8_t	res;

	res = 1;
	little_endian = magic == FAT_MAGIC || magic == FAT_MAGIC_64 ? 0 : 1;
	if (magic == FAT_MAGIC)
		ft_printf("32bits FAT file, I'm not treated yet\n");
	else if (magic == FAT_MAGIC_64)
		ft_printf("64bits FAT file, I'm not treated yet\n");
	else if (magic == FAT_CIGAM)
		ft_printf("32bits little endian file, development in progress...\n");
	else if (magic == FAT_CIGAM_64)
		ft_printf("64bits little endian file, I'm not treated yet\n");
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		res = handle_fat_32(arg, ptr, little_endian);
	return (res);
}
