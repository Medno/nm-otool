/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_symbols.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 13:25:49 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/27 17:51:27 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

char	find_segment_type(struct nlist_64 list, char *ptr)
{
	uint32_t				i;
	struct mach_header_64	*macho_64;
	struct load_command		*lc;
	struct segment_command_64	*seg;
	uint32_t				count_sect;

	(void)list;
	i = 0;
	count_sect = 0;
	macho_64 = (struct mach_header_64 *)ptr;
	lc = (struct load_command *)(ptr + sizeof(*macho_64));
	seg = NULL;
	while (i < macho_64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			seg = (struct segment_command_64 *)lc;
			ft_printf("Name of segment : %s\n", seg->segname);
			count_sect += seg->nsects;

		}
		if (list.n_sect <= count_sect)
			break ;
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
			ft_printf("Name of segment : %s\n", seg->segname);
	if (seg && seg->segname == SEG_TEXT)
		return ('T');
	if (seg && seg->segname == SEG_DATA)
		return ('D');
	if (seg && seg->segname == SECT_BSS)
		return ('B');
	return ('?');
}

char	find_n_type(struct nlist_64 list, char *ptr)
{
	if (list.n_type & N_TYPE)
	{
		if ((list.n_type & N_TYPE) == N_UNDF)
		{
			if (list.n_value != 0)
				return ('C');
			return ('U');
		}
		else if ((list.n_type & N_TYPE) == N_PBUD)
			return ('U');
		else if ((list.n_type & N_TYPE) == N_ABS)
			return ('A');
		else if ((list.n_type & N_TYPE) == N_SECT)
		{
			return (find_segment_type(list, ptr));
		}
		else if ((list.n_type & N_TYPE) == N_INDR)
			return ('I');
	}
	return ('U');
}

void	display_sym_tab(char *ptr, struct load_command *lc)
{
	uint32_t				i;
	char					*str_tab;
	struct symtab_command	*sc;
	struct nlist_64			*nlist;
	char					type;

	i = 0;
	sc = (struct symtab_command *)lc;
	nlist = (struct nlist_64 *)(ptr + sc->symoff);
	str_tab = (char *)(ptr + sc->stroff);
	while (i < sc->nsyms)
	{
		type = find_n_type(nlist[i], ptr);
		if (nlist[i].n_value)
			ft_printf("%016llx %c %s\n", nlist[i].n_value, type,
			str_tab + nlist[i].n_un.n_strx);
		else
			ft_printf("%16s %c %s\n", "", type,
			str_tab + nlist[i].n_un.n_strx);
//		ft_printf("%x %x\n", sc->symoff, sc->stroff);
		i++;
	}
}

uint8_t	list_symbols(char *ptr)
{
	struct mach_header_64	*macho_64;
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
ft_printf("Starting listing symbols...\n");
	macho_64 = (struct mach_header_64 *)ptr;
	lc = (struct load_command*)(ptr + sizeof(*macho_64));
	while (i < macho_64->ncmds)
	{
		if (lc->cmd == LC_SEGMENT_64)
		{
			ft_printf("Segment\n");
		}
		if (lc->cmd == LC_SYMTAB)
		{
			ft_printf("Symbols table\n");
			display_sym_tab(ptr, lc);
		}
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	
	return (0);
}

uint8_t	handle_architecture(char *ptr)
{
	uint32_t	magic;

	magic = *(int *)ptr;
	if (magic == MH_MAGIC_64)
	{
		ft_printf("64bit MACH-O file, development in progress...\n");
		return list_symbols(ptr);
	}
	else if (magic == MH_MAGIC)
		ft_printf("32bit MACH-O file, I'm not treated yet\n");
	else if (magic == FAT_MAGIC_64)
		ft_printf("64bit FAT file, I'm not treated yet\n");
	else if (magic == FAT_MAGIC)
		ft_printf("32bit FAT file, I'm not treated yet\n");
	return (0);
}
