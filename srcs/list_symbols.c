/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/01 19:00:57 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	handle_32(t_symbols *sym, t_sc *sc)
{
	t_ulist		new[sym->n_syms];
	char		*str_tab;
	t_n32		*nl_32;
	uint32_t	i;

	i = 0;
	ft_printf("Start handling 32 bits\n");
	nl_32 = (t_n32 *)(sym->header_ptr + sc->symoff);
	str_tab = (char *)(sym->header_ptr + sc->stroff);
	while (i < sym->n_syms)
	{
		new[i].nl.n_un.n_strx = nl_32[i].n_un.n_strx;
		new[i].nl.n_type = nl_32[i].n_type;
		new[i].nl.n_sect = nl_32[i].n_sect;
		new[i].nl.n_desc = nl_32[i].n_desc;
		new[i].nl.n_value = nl_32[i].n_value;
	ft_printf("Value of nstrx : |%08llx| |%s|\n", new[i].nl.n_value, str_tab + new[i].nl.n_un.n_strx);
	ft_printf("Value of nstrx : |%08llx| |%s|\n", nl_32[i].n_value, str_tab + nl_32[i].n_un.n_strx);
		i++;
	}
	m_sort(new, str_tab, 0, sym->n_syms - 1);
	print_symbols(sym, new, str_tab);
}


void	handle_64(t_symbols *sym, t_sc *sc)
{
	t_ulist		new[sym->n_syms];
	char		*str_tab;
	t_n64		*nl_64;
	uint32_t	i;

	i = 0;
	ft_printf("Start handling 64 bits\n");
	nl_64 = (t_n64 *)(sym->header_ptr + sc->symoff);
	str_tab = (char *)(sym->header_ptr + sc->stroff);
	while (i < sym->n_syms)
	{
		new[i].nl = nl_64[i];
		i++;
	}
	m_sort(new, str_tab, 0, sym->n_syms - 1);
	print_symbols(sym, new, str_tab);
}

void	display_sym_tab(t_symbols *sym, struct load_command *lc)
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

void	add_sect_in_struct(t_symbols *sym, struct load_command *lc)
{
	struct segment_command_64	*sc;
	struct section_64			*sect_64;
	uint32_t					i;

	ft_printf("Start adding sections for 64bits...\n");
	sc = (struct segment_command_64 *)lc;
	if (sc->nsects > 0)
	{
		i = 0;
		sect_64 = (struct section_64 *)((char *)sc + sizeof(*sc));
		while (i < sc->nsects)
		{
			sym->sections[sym->n_sects + i].name = (sect_64 + i)->sectname;
			sym->sections[sym->n_sects + i].seg_name = (sect_64 + i)->segname;
			i++;
		}
		sym->n_sects += sc->nsects;
	}
}

void	add_sect_in_struct_32(t_symbols *sym, struct load_command *lc)
{
	struct segment_command	*sc;
	struct section			*sect_32;
	uint32_t					i;

	ft_printf("Start adding sections for 32bits...\n");
	sc = (struct segment_command *)lc;
	if (sc->nsects > 0)
	{
		i = 0;
		sect_32 = (struct section *)((char *)sc + sizeof(*sc));
		while (i < sc->nsects)
		{
			sym->sections[sym->n_sects + i].name = (sect_32 + i)->sectname;
			sym->sections[sym->n_sects + i].seg_name = (sect_32 + i)->segname;
			i++;
		}
		sym->n_sects += sc->nsects;
	}
}

void	fill_struct(t_symbols *sym)
{
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
	lc = sym->lc;
	ft_printf("Start filling structure...\n");
	while (i < sym->n_cmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			add_sect_in_struct(sym, lc);
		else if (lc->cmd == LC_SEGMENT)
			add_sect_in_struct_32(sym, lc);
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
}

uint8_t	list_symbols(t_symbols *sym)
{
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
	ft_printf("Starting listing symbols...\n");
	fill_struct(sym);
	lc = sym->lc;
	while (i < sym->n_cmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
		}
		if (lc->cmd == LC_SYMTAB)
			display_sym_tab(sym, lc);
		if (lc->cmd == LC_DYSYMTAB)
			ft_printf("DYSYMTAB\n");

		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	return (0);
}

t_symbols	init_symbols_struct(char *arg, char *ptr)
{
	t_symbols symbols;

	symbols.magic = *(int *)ptr;
	symbols.file_name = arg;
	symbols.header_ptr = ptr;
	symbols.n_sects = 0;
	symbols.n_syms = 0;
	if (symbols.magic == MH_MAGIC)
	{
		symbols.lc = (t_lc *)(symbols.header_ptr
				+ sizeof(struct mach_header));
		symbols.n_cmds = ((struct mach_header *)(symbols.header_ptr))->ncmds;
	}
	else
	{
		symbols.lc = (t_lc *)(symbols.header_ptr
				+ sizeof(struct mach_header_64));
		symbols.n_cmds = ((struct mach_header_64 *)(symbols.header_ptr))->ncmds;
	}
	return (symbols);
}

uint8_t	handle_architecture(char *arg, char *ptr)
{
	t_symbols	symbols;

	symbols = init_symbols_struct(arg, ptr);
	if (symbols.magic == MH_MAGIC_64)
	{
		ft_printf("64bit MACH-O file, development in progress...\n");
		int ret = list_symbols(&symbols);
		print_struct_sym(symbols);
		return (ret);
	}
	else if (symbols.magic == MH_MAGIC)
	{
		ft_printf("32bit MACH-O file, I'm not treated yet\n");
		int ret = list_symbols(&symbols);
		print_struct_sym(symbols);
		return (ret);

	}
	else if (symbols.magic == FAT_MAGIC_64)
		ft_printf("64bit FAT file, I'm not treated yet\n");
	else if (symbols.magic == FAT_CIGAM)
		ft_printf("32bit little endian file, I'm not treated yet\n");
	else if (symbols.magic == FAT_CIGAM_64)
		ft_printf("64bit little endian file, I'm not treated yet\n");
	else if (symbols.magic == FAT_MAGIC)
		ft_printf("32bit FAT file, I'm not treated yet\n");
	return (0);
}
