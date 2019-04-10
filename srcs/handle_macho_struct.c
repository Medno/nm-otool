/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:05:35 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/10 17:40:38 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	handle_32(t_finfo file, t_fhead *head, t_sc *sc)
{
	t_ulist		new[head->macho.n_syms];
	char		*str_tab;
	t_n32		*nl_32;
	t_point_32	ij;

	ij.x = 0;
	ij.y = 0;
	nl_32 = (t_n32 *)(head->current + to_big_endian(head->macho.l_endian, sc->symoff));
	str_tab = (char *)(head->current + to_big_endian(head->macho.l_endian, sc->stroff));
	if (!(nl_32 && str_tab))
		return (1);
	while (ij.x < head->macho.n_syms)
	{
		if (to_big_endian(head->macho.l_endian, nl_32->n_un.n_strx)
				> to_big_endian(head->macho.l_endian, sc->strsize))
			return (1);
		//if (!(to_big_endian(head->macho.l_endian, nl_32->n_type) & N_STAB))
		if (!(nl_32->n_type & N_STAB))
		{
			new[ij.y].nl.n_un.n_strx
				= to_big_endian(head->macho.l_endian, nl_32->n_un.n_strx);
			new[ij.y].nl.n_type = nl_32->n_type;
			new[ij.y].nl.n_sect = nl_32->n_sect;
			new[ij.y].nl.n_desc = to_big_endian(head->macho.l_endian, nl_32->n_desc);
			new[ij.y].nl.n_value = to_big_endian(head->macho.l_endian, nl_32->n_value);
			ij.y++;
		}
		nl_32 = (void *)nl_32 + sizeof(*nl_32);
		ij.x++;
	}
	m_sort(new, str_tab, 0, ij.y - 1);
	head->macho.n_syms = ij.y;
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
ft_printf("handle_64 1\n");
	nl_64 = (t_n64 *)(head->current + sc->symoff);
	str_tab = (char *)(head->current + sc->stroff);
ft_printf("handle_64 2\n");
	if (!(nl_64 && str_tab))
		return (1);
	while (i < head->macho.n_syms)
	{
		if (nl_64[i].n_un.n_strx > sc->strsize)
			return (1);
ft_printf("handle_64 3\n");
		new[i].nl = nl_64[i];
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

	sc = (struct segment_command_64 *)lc;
	if (!sc)
		return (1);
	if (sc->nsects > 0)
	{
		i = 0;
		sect_64 = (struct section_64 *)((char *)sc + sizeof(*sc));
		if (sect_64 && (char *)sc + sizeof(*sect_64) > head->ptr + file.size)
			return (1);
		while (i < sc->nsects)
		{
			head->macho.sect[head->macho.n_sects + i].name =
				(sect_64 + i)->sectname;
			head->macho.sect[head->macho.n_sects + i].seg_name =
				(sect_64 + i)->segname;
			i++;
		}
		head->macho.n_sects += sc->nsects;
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
	if (!sc)
		return (1);
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
			head->macho.sect[head->macho.n_sects + i].seg_name
				= (sect_32 + i)->segname;
			i++;
		}
		head->macho.n_sects += sc_nsects;
	}
	return (0);
}
