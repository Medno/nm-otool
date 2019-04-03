/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/03 19:45:07 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	handle_32(t_symbols *sym, t_sc *sc)
{
	t_ulist		new[sym->n_syms];
	char		*str_tab;
	t_n32		*nl_32;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 0;
	ft_printf("Start handling 32 bits\n");
	nl_32 = (t_n32 *)(sym->header_ptr + sc->symoff);
	str_tab = (char *)(sym->header_ptr + sc->stroff);
	while (i < sym->n_syms)
	{
		if (!(nl_32[i].n_type & N_STAB))
		{
			new[j].nl.n_un.n_strx = nl_32[i].n_un.n_strx;
			new[j].nl.n_type = nl_32[i].n_type;
			new[j].nl.n_sect = nl_32[i].n_sect;
			new[j].nl.n_desc = nl_32[i].n_desc;
			new[j].nl.n_value = nl_32[i].n_value;
			j++;
		}
		i++;
	}

	ft_printf("Start sorting...\n");
	m_sort(new, str_tab, 0, j - 1);
	sym->n_syms = j;
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

	ft_printf("Handling 32 / 64 bit mach o header\n");
	i = 0;
	sc = (t_sc *)lc;
	sym->n_syms = sc->nsyms;
	if (sym->magic == MH_MAGIC)
		handle_32(sym, sc);
	else if (sym->magic == MH_MAGIC_64)
		handle_64(sym, sc);
}

void	add_sect_in_struct_64(t_symbols *sym, struct load_command *lc)
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
	uint32_t				lc_cmdsize;

print_struct_sym(*sym);
	i = 0;
	lc = sym->lc;
	ft_printf("Start filling structure...\n");
	while (i < sym->n_cmds)
	{
		ft_printf("test\n");
		lc_cmdsize = to_big_endian(sym->l_endian, lc->cmdsize);
		ft_printf("Size of load command: |%zu|\n", lc_cmdsize);
		if (lc->cmd == LC_SEGMENT_64)
			add_sect_in_struct_64(sym, lc);
		else if (lc->cmd == LC_SEGMENT)
			add_sect_in_struct_32(sym, lc);
		ft_printf("test 1\n");
		lc = (void *)lc + lc_cmdsize;
		ft_printf("test 2\n");
		i++;
	}
	ft_printf("end of filling\n");
}

uint8_t	list_symbols(char *arg, void *ptr)
{
	struct load_command		*lc;
	t_symbols				sym;
	uint32_t				i;
	uint32_t				lc_cmdsize;

	i = 0;
	sym = init_symbols_struct(arg, ptr);
	ft_printf("Starting listing symbols...\n");
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
print_struct_sym(sym);
	return (0);
}

uint8_t	handle_architecture(char *arg, char *ptr)
{
	uint32_t	magic;

	magic = *(uint32_t *)ptr;
	if (invalid_filetype(ptr, magic))
		return (handle_error(arg));
	if (magic == FAT_MAGIC || magic == FAT_MAGIC_64 || magic == FAT_CIGAM
			|| magic == FAT_CIGAM_64)
		return (handle_fat(arg, ptr, magic));
	if (magic == MH_MAGIC_64 || magic == MH_MAGIC)
	{
		ft_printf("64bits or 32bits MACH-O file\n");
		return (list_symbols(arg, ptr));
	}
	else
		handle_archive(arg, ptr);
	return (0);
}
