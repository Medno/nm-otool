/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 13:42:38 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	get_all_syms(t_symbols *sym, t_n64 *nl, uint32_t nsyms, char *str_tab)
{
	t_n64		*new[nsyms];
	uint32_t	i;
	char		type;

	i = 0;
	while (i < nsyms)
	{
		new[i] = &nl[i];
		i++;
	}
	m_sort(new, str_tab, 0, nsyms - 1);
	i = 0;
	while (i < nsyms)
	{
		type = find_symbol_type(sym, *new[i]);
		if (new[i]->n_value)
			ft_printf("%016llx %c %s\n", new[i]->n_value, type,
					str_tab + new[i]->n_un.n_strx);
		else
			ft_printf("%16s %c %s\n", "", type,
					str_tab + new[i]->n_un.n_strx);
		i++;
	}
}

void	display_sym_tab(t_symbols *sym, struct load_command *lc)
{
	uint32_t				i;
	char					*str_tab;
	struct symtab_command	*sc;
	struct nlist_64			*nlist;

	i = 0;
	sc = (struct symtab_command *)lc;
	nlist = (struct nlist_64 *)(sym->header_ptr + sc->symoff);
	str_tab = (char *)(sym->header_ptr + sc->stroff);
	get_all_syms(sym, nlist, sc->nsyms, str_tab);
}

void	add_sect_in_struct(t_symbols *sym, struct load_command *lc)
{
	struct segment_command_64	*sc;
	struct section_64			*sect_64;
	uint32_t					i;

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

void	fill_struct(t_symbols *sym, struct mach_header_64 *macho_64)
{
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
	macho_64 = (struct mach_header_64 *)sym->header_ptr;
	lc = (struct load_command*)(sym->header_ptr + sizeof(*macho_64));
	while (i < macho_64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
			add_sect_in_struct(sym, lc);
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
}

uint8_t	list_symbols(t_symbols *sym)
{
	struct mach_header_64	*macho_64;
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
	ft_printf("Starting listing symbols...\n");
	macho_64 = (struct mach_header_64 *)sym->header_ptr;
	lc = (struct load_command*)(sym->header_ptr + sizeof(*macho_64));
	fill_struct(sym, macho_64);
	while (i < macho_64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
		}
		if (lc->cmd == LC_SYMTAB)
		{
			display_sym_tab(sym, lc);
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	return (0);
}

uint8_t	handle_architecture(char *arg, char *ptr)
{
	uint32_t	magic;
	t_symbols	symbols;

	magic = *(int *)ptr;
	symbols.header_ptr = ptr;
	symbols.n_cmds = 0;
	symbols.n_sects = 0;
	symbols.file_name = arg;
	if (magic == MH_MAGIC_64)
	{
		ft_printf("64bit MACH-O file, development in progress...\n");
		int ret = list_symbols(&symbols);
		print_struct_sym(symbols);
		return (ret);
	}
	else if (magic == MH_MAGIC)
		ft_printf("32bit MACH-O file, I'm not treated yet\n");
	else if (magic == FAT_MAGIC_64)
		ft_printf("64bit FAT file, I'm not treated yet\n");
	else if (magic == FAT_MAGIC)
		ft_printf("32bit FAT file, I'm not treated yet\n");
	return (0);
}
