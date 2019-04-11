/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:05:35 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/11 16:21:47 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	copy_32(t_fhead *head, t_ulist new[], t_sc *sc)
{
	uint32_t	i;
	t_n32		*nl;

	i = 0;
	nl = (t_n32 *)(head->current
			+ to_big_endian(head->macho.l_endian, sc->symoff));
	while (i < head->macho.n_syms)
	{
		if (!nl || to_big_endian(head->macho.l_endian, nl->n_un.n_strx)
				> to_big_endian(head->macho.l_endian, sc->strsize))
			return (1);
		new[i].nl.n_un.n_strx =
			to_big_endian(head->macho.l_endian, nl->n_un.n_strx);
		new[i].nl.n_type = nl->n_type;
		new[i].nl.n_sect = nl->n_sect;
		new[i].nl.n_desc = to_big_endian(head->macho.l_endian, nl->n_desc);
		new[i].nl.n_value = to_big_endian(head->macho.l_endian, nl->n_value);
		nl = (void *)nl + sizeof(*nl);
		i++;
	}
	return (0);
}

uint8_t	handle_32(t_finfo file, t_fhead *head, t_sc *sc)
{
	t_ulist		new[head->macho.n_syms];
	char		*str_tab;

	str_tab = (char *)(head->current
			+ to_big_endian(head->macho.l_endian, sc->stroff));
	if (!str_tab || str_tab > head->ptr + file.size)
		return (1);
	if (copy_32(head, new, sc))
		return (1);
	m_sort(new, str_tab, 0, head->macho.n_syms - 1);
	print_symbols(file, head, new, str_tab);
	return (0);
}

uint8_t	handle_64(t_finfo file, t_fhead *head, t_sc *sc)
{
	t_ulist		new[head->macho.n_syms];
	char		*str_tab;
	t_n64		*nl_64;
	uint32_t	i;

	i = 0;
	nl_64 = (t_n64 *)(head->current
			+ to_big_endian(head->macho.l_endian, sc->symoff));
	str_tab = (char *)(head->current
			+ to_big_endian(head->macho.l_endian, sc->stroff));
	if (!(nl_64 && str_tab) || str_tab > head->ptr + file.size)
		return (1);
	while (i < head->macho.n_syms)
	{
		if (!nl_64 || nl_64->n_un.n_strx
				> to_big_endian(head->macho.l_endian, sc->strsize))
			return (1);
		new[i].nl = *nl_64;
		nl_64 = (void *)nl_64 + sizeof(*nl_64);
		i++;
	}
	m_sort(new, str_tab, 0, head->macho.n_syms - 1);
	print_symbols(file, head, new, str_tab);
	return (0);
}

uint8_t	add_sect_64(t_finfo file, t_fhead *head, t_lc *lc)
{
	struct segment_command_64	*sc;
	struct section_64			*sect_64;
	uint32_t					i;
	uint32_t					sc_nsects;

	sc = (struct segment_command_64 *)lc;
	sc_nsects = to_big_endian(head->macho.l_endian, sc->nsects);
	if (sc_nsects > 0)
	{
		i = 0;
		sect_64 = (struct section_64 *)((char *)sc + sizeof(*sc));
		if (sect_64 && (char *)sc + sizeof(*sect_64) > head->ptr + file.size)
			return (1);
		while (i < sc_nsects)
		{
			head->macho.sect[head->macho.n_sects + i].name =
				(sect_64 + i)->sectname;
			head->macho.sect[head->macho.n_sects + i].seg_name =
				(sect_64 + i)->segname;
			i++;
		}
		head->macho.n_sects += sc_nsects;
	}
	return (0);
}

uint8_t	add_sect_32(t_finfo file, t_fhead *head, t_lc *lc)
{
	struct segment_command	*sc;
	struct section			*sect_32;
	uint32_t				i;
	uint32_t				sc_nsects;

	sc = (struct segment_command *)lc;
	sc_nsects = to_big_endian(head->macho.l_endian, sc->nsects);
	if (sc_nsects > 0)
	{
		i = 0;
		sect_32 = (struct section *)((char *)sc + sizeof(*sc));
		if (sect_32 && (char *)sc + sizeof(*sect_32) > head->ptr + file.size)
			return (1);
		while (i < sc_nsects)
		{
			head->macho.sect[head->macho.n_sects + i].name =
				(sect_32 + i)->sectname;
			head->macho.sect[head->macho.n_sects + i].seg_name =
				(sect_32 + i)->segname;
			i++;
		}
		head->macho.n_sects += sc_nsects;
	}
	return (0);
}
