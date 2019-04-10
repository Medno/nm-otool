/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:21:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/10 14:47:47 by pchadeni         ###   ########.fr       */
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

uint8_t	assign_mach_headers_details(t_finfo file, t_fhead *head)
{
	t_mh	*mh;
	t_mh64	*mh_64;

	if (head->macho.magic == MH_MAGIC || head->macho.magic == MH_CIGAM)
	{
		if (head->current + sizeof(t_mh) > head->current + file.size)
			return (1);
		mh = (t_mh *)head->current;
		macho_details(head, mh->sizeofcmds, mh->cputype, mh->ncmds);
		head->macho.lc = (t_lc *)(head->current + sizeof(t_mh));
	}
	else
	{
		if (head->current + sizeof(t_mh64) > head->current + file.size)
			return (1);
		mh_64 = (t_mh64 *)head->current;
		macho_details(head, mh_64->sizeofcmds, mh_64->cputype, mh_64->ncmds);
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
