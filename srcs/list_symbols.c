/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/30 16:28:10 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	display_sym_tab(t_finfo file, t_fhead *head, t_lc *lc)
{
	uint32_t	i;
	t_sc		*sc;

	i = 0;
	sc = (t_sc *)lc;
	head->macho.n_syms = to_big_endian(head->macho.l_endian, sc->nsyms);
	head->macho.len_value = 0;
	if (handle_macho(file, head, sc))
		return (handle_error(file.name, E_CORRUPT, head->opts));
	return (0);
}

uint8_t	fill_struct(t_finfo file, t_fhead *head, t_lc *lc)
{
	uint32_t	i;
	uint32_t	lc_cmdsize;
	uint32_t	lc_cmd;

	i = 0;
	while (i < head->macho.header.ncmds)
	{
		lc_cmd = to_big_endian(head->macho.l_endian, lc->cmd);
		lc_cmdsize = to_big_endian(head->macho.l_endian, lc->cmdsize);
		if ((char *)lc + lc_cmdsize > (char *)head->macho.lc +
		head->macho.header.sizeofcmds || (head->macho.is64 && lc_cmdsize % 8)
		|| (!head->macho.is64 && lc_cmdsize % 4))
			return (1);
		if (lc_cmd == LC_SEGMENT_64 && add_sect_64(file, head, lc))
			return (1);
		else if (lc_cmd == LC_SEGMENT && add_sect_32(file, head, lc))
			return (1);
		lc = (void *)lc + lc_cmdsize;
		i++;
	}
	return (0);
}

uint8_t	list_symbols(t_finfo file, t_fhead *head, char *obj_n)
{
	t_lc		*lc;
	uint32_t	i;
	uint32_t	lc_cmdsize;

	i = 0;
	if (init_symbols_struct(file, head, obj_n))
		return (handle_error(file.name, E_CORRUPT, head->opts));
	lc = head->macho.lc;
	if (fill_struct(file, head, lc))
		return (handle_error(file.name, E_CORRUPT, head->opts));
	while (i < head->macho.header.ncmds)
	{
		lc_cmdsize = to_big_endian(head->macho.l_endian, lc->cmdsize);
		if (to_big_endian(head->macho.l_endian, lc->cmd) == LC_SYMTAB)
			if (display_sym_tab(file, head, lc))
				return (1);
		lc = (void *)lc + lc_cmdsize;
		i++;
	}
	if (!head->macho.n_syms)
		print_symbols(file, head);
	if (head->opts & OPT_H)
		return (display_mach_header(head));
	return (0);
}

uint8_t	handle_architecture(char *arg, char *ptr, int size, uint16_t opts)
{
	t_finfo		file;
	t_fhead		headers;
	uint32_t	magic;

	magic = *(uint32_t *)ptr;
	file.name = arg;
	file.size = size;
	headers.opts = opts;
	if (((magic == MH_MAGIC || magic == MH_CIGAM) && ptr + sizeof(t_mh) >
		ptr + size) || ((magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
		&& ptr + sizeof(t_mh64) > ptr + size))
		return (handle_error(file.name, E_CORRUPT, headers.opts));
	headers.archive = 0;
	headers.fat = 0;
	headers.ptr = ptr;
	headers.current = ptr;
	headers.fat_arch = NULL;
	headers.macho.n_syms = 0;
	if (invalid_filetype(ptr))
		return (handle_error(file.name, E_NOT_OBJ, headers.opts));
	if (is_fat(magic))
		return (handle_fat(file, &headers, magic));
	else if (is_macho(magic))
		return (list_symbols(file, &headers, file.name));
	return (handle_archive(file, &headers));
}
