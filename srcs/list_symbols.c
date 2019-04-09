/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/09 15:29:30 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	display_sym_tab(t_symbols *sym, t_lc *lc)
{
	uint32_t	i;
	t_sc		*sc;

	i = 0;
	sc = (t_sc *)lc;
	sym->n_syms = sc->nsyms;
	if (sym->magic == MH_MAGIC)
		handle_32(sym, sc);
	else if (sym->magic == MH_MAGIC_64)
		handle_64(sym, sc);
}

uint8_t	fill_struct(t_symbols *sym)
{
	t_lc		*lc;
	uint32_t	i;
	uint32_t	lc_cmdsize;

	i = 0;
	/*
	if (sizeof(t_lc) + ptr + sizzeof(struct mach_header) > file.size)
		return (1);
	*/
	lc = sym->lc;
	while (i < sym->n_cmds)
	{
		/*
		if (sizeof(t_lc) + ptr + sizzeof(struct mach_header) > file.size)
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

uint8_t	list_symbols(char *arg, void *ptr)
{
	t_lc		*lc;
	t_symbols	sym;
	uint32_t	i;
	uint32_t	lc_cmdsize;

	i = 0;
	sym = init_symbols_struct(arg, ptr);
	fill_struct(&sym);
	lc = sym.lc;
	while (i < sym.n_cmds)
	{
		lc_cmdsize = to_big_endian(sym.l_endian, lc->cmdsize);
		if (lc->cmd == LC_SYMTAB)
			display_sym_tab(&sym, lc);
		lc = (void *)lc + lc_cmdsize;
		i++;
	}
	return (0);
}

uint8_t	handle_architecture(char *arg, char *ptr, int size, uint8_t opts)
{
	t_finfo		file;
	uint32_t	magic;

	magic = *(uint32_t *)ptr;
	file.name = arg;
	file.size = size;
	file.opts = opts;
	ft_printf("Size of file : %d\n", size);
	if (ptr + sizeof(struct mach_header) > ptr + file.size)
		return (1);
	if (invalid_filetype(ptr))
		return (handle_error(arg, E_UNDIF_FILE));
	if (is_fat(magic))
		return (handle_fat(arg, ptr, magic));
	else if (is_macho(magic))
		return (list_symbols(arg, ptr));
	return (handle_archive(arg, ptr));
}
