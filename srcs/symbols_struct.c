/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:21:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/29 18:30:11 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	macho_details(t_fhead *head, uint32_t s, cpu_type_t c, uint32_t n)
{
	head->macho.s_lc = to_big_endian(head->macho.l_endian, s);
	head->macho.cpu_type = to_big_endian(head->macho.l_endian, c);
	head->macho.n_cmds = to_big_endian(head->macho.l_endian, n);
	return (0);
}

uint8_t	first_copy(t_fhead *head, cpu_type_t c, cpu_subtype_t s, uint32_t f)
{
	head->macho.header.cputype = to_big_endian(head->macho.l_endian, c);
	head->macho.header.cpusubtype = to_big_endian(head->macho.l_endian, s);
	head->macho.header.filetype = to_big_endian(head->macho.l_endian, f);
	return (0);
}

uint8_t	second_copy(t_fhead *head, uint32_t n, uint32_t s, uint32_t fl)
{
	head->macho.header.ncmds = to_big_endian(head->macho.l_endian, n);
	head->macho.header.sizeofcmds = to_big_endian(head->macho.l_endian, s);
	head->macho.header.flags = to_big_endian(head->macho.l_endian, fl);
	return (0);
}

uint8_t	assign_mach_headers_details(t_finfo file, t_fhead *head)
{
	t_mh	*mh;
	t_mh64	*mh_64;

	head->macho.header.magic = to_big_endian(head->macho.l_endian, (uint64_t)head->macho.magic);
	if (head->macho.magic == MH_MAGIC || head->macho.magic == MH_CIGAM)
	{
		if (head->current + sizeof(t_mh) > head->current + file.size)
			return (1);
		mh = (t_mh *)head->current;
		macho_details(head, mh->sizeofcmds, mh->cputype, mh->ncmds);
		first_copy(head, mh->cputype, mh->cpusubtype, mh->filetype);
		second_copy(head, mh->ncmds, mh->sizeofcmds, mh->flags);
		head->macho.lc = (t_lc *)(head->current + sizeof(t_mh));
	}
	else
	{
		if (head->current + sizeof(t_mh64) > head->current + file.size)
			return (1);
		mh_64 = (t_mh64 *)head->current;
		macho_details(head, mh_64->sizeofcmds, mh_64->cputype, mh_64->ncmds);
		first_copy(head, mh_64->cputype, mh_64->cpusubtype, mh_64->filetype);
		second_copy(head, mh_64->ncmds, mh_64->sizeofcmds, mh_64->flags);
		head->macho.lc = (t_lc *)(head->current + sizeof(t_mh64));
	}
	return (head->current + sizeof(t_lc) > head->current + file.size
		|| head->current + head->macho.s_lc > head->current + file.size);
}

uint8_t	init_symbols_struct(t_finfo file, t_fhead *head, char *name)
{
	uint32_t	magic;

	head->macho.obj_name = name;
	magic = *(int *)(head->current);
	head->macho.magic = magic;
	head->macho.n_sects = 0;
	head->macho.n_syms = 0;
	head->macho.is64 = magic == MH_MAGIC_64 || magic == MH_CIGAM_64;
	head->macho.l_endian = magic == MH_CIGAM || magic == MH_CIGAM_64;
	return (assign_mach_headers_details(file, head));
}
