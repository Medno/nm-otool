/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:05:35 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 17:07:04 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	handle_32(t_symbols *sym, t_sc *sc)
{
	t_ulist		new[sym->n_syms];
	char		*str_tab;
	t_n32		*nl_32;
	t_point_32	ij;

	ij.x = 0;
	ij.y = 0;
	nl_32 = (t_n32 *)(sym->header_ptr + sc->symoff);
	str_tab = (char *)(sym->header_ptr + sc->stroff);
	while (ij.x < sym->n_syms)
	{
		if (!(nl_32[ij.x].n_type & N_STAB))
		{
			new[ij.y].nl.n_un.n_strx = nl_32[ij.x].n_un.n_strx;
			new[ij.y].nl.n_type = nl_32[ij.x].n_type;
			new[ij.y].nl.n_sect = nl_32[ij.x].n_sect;
			new[ij.y].nl.n_desc = nl_32[ij.x].n_desc;
			new[ij.y].nl.n_value = nl_32[ij.x].n_value;
			ij.y++;
		}
		ij.x++;
	}
	m_sort(new, str_tab, 0, ij.y - 1);
	sym->n_syms = ij.y;
	print_symbols(sym, new, str_tab);
}

void	handle_64(t_symbols *sym, t_sc *sc)
{
	t_ulist		new[sym->n_syms];
	char		*str_tab;
	t_n64		*nl_64;
	uint32_t	i;

	i = 0;
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

void	add_sect_in_struct_64(t_symbols *sym, t_lc *lc)
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

void	add_sect_in_struct_32(t_symbols *sym, t_lc *lc)
{
	struct segment_command	*sc;
	struct section			*sect_32;
	uint32_t				i;

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
