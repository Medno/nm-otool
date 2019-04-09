/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/09 19:19:29 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	display_sym_tab(t_finfo file, t_fhead *head, t_lc *lc)
{
	uint32_t	i;
	t_sc		*sc;
(void)file;
	i = 0;
	sc = (t_sc *)lc;
	head->macho.n_syms = sc->nsyms;
	if (head->macho.magic == MH_MAGIC)
		handle_32(file, head, sc);
	else if (head->macho.magic == MH_MAGIC_64)
		handle_64(file, head, sc);
}

uint8_t	fill_struct(t_symbols *sym)
{
	t_lc		*lc;
	uint32_t	i;
	uint32_t	lc_cmdsize;

	i = 0;
	/*
	if (sizeof(t_lc) + ptr + sizeof(struct mach_header) > file.size)
		return (1);
	*/
	lc = sym->lc;
	while (i < sym->n_cmds)
	{
		/*
		if (sizeof(t_lc) + ptr + sizeof(struct mach_header) > file.size)
			return (1);
		*/
		lc_cmdsize = to_big_endian(sym->l_endian, lc->cmdsize);
		/*
		if (lc_cmdsize % 4 || lc_cmdsize % 8)
			return (1);
		*/
		if (lc->cmd == LC_SEGMENT_64)
			add_sect_in_struct_64(sym, lc);
		else if (lc->cmd == LC_SEGMENT)
			add_sect_in_struct_32(sym, lc);
		lc = (void *)lc + lc_cmdsize;
		i++;
	}
	return (0);
}

uint8_t	list_symbols(t_finfo file, t_fhead *head, char *obj_n)
{
	t_lc		*lc;
//	t_symbols	sym;
	uint32_t	i;
	uint32_t	lc_cmdsize;

	i = 0;
	head->macho = init_symbols_struct(obj_n, head->current);
	fill_struct(&head->macho);
	lc = head->macho.lc;
	while (i < head->macho.n_cmds)
	{
		lc_cmdsize = to_big_endian(head->macho.l_endian, lc->cmdsize);
		if (lc->cmd == LC_SYMTAB)
			display_sym_tab(file, head, lc);
		lc = (void *)lc + lc_cmdsize;
		i++;
	}
	return (0);
}

uint8_t	handle_architecture(char *arg, char *ptr, int size, uint8_t opts)
{
	t_finfo		file;
	t_fhead		headers;
	uint32_t	magic;

	magic = *(uint32_t *)ptr;
	if ((magic == MH_MAGIC || magic == MH_CIGAM)
			&& ptr + sizeof(struct mach_header) > ptr + size)
		return (1);
	if ((magic == MH_MAGIC_64 || magic == MH_CIGAM_64)
			&& ptr + sizeof(struct mach_header_64) > ptr + size)
		return (1);
	file.name = arg;
	file.size = size;
	file.opts = opts;
	file.arch = NXGetLocalArchInfo();
	headers.archive = 0;
	headers.fat = 0;
	headers.ptr = ptr;
	headers.current = ptr;
	if (invalid_filetype(ptr))
		return (handle_error(arg, E_UNDIF_FILE));
	if (is_fat(magic))
		return (handle_fat(file, &headers, magic));
	else if (is_macho(magic))
		return (list_symbols(file, &headers, file.name));
	return (handle_archive(file, &headers));
}
