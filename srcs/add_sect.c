/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_sect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 12:26:34 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 12:27:08 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	add_values(t_fhead *h, uint32_t i, uint64_t addr, uint64_t size)
{
	h->macho.sect[h->macho.n_sects + i].addr =
		to_big_endian(h->macho.l_endian, addr);
	h->macho.sect[h->macho.n_sects + i].size =
		to_big_endian(h->macho.l_endian, size);
}

uint8_t	add_sect_64(t_finfo file, t_fhead *h, t_lc *lc)
{
	struct segment_command_64	*sc;
	struct section_64			*s64;
	uint32_t					i;
	uint32_t					sc_nsects;

	sc = (struct segment_command_64 *)lc;
	sc_nsects = to_big_endian(h->macho.l_endian, sc->nsects);
	if (sc_nsects > 0)
	{
		i = 0;
		s64 = (struct section_64 *)((char *)sc + sizeof(*sc));
		if (s64 && (char *)sc + sizeof(*s64) > h->ptr + file.size)
			return (1);
		while (i < sc_nsects)
		{
			h->macho.sect[h->macho.n_sects + i].name = (s64 + i)->sectname;
			h->macho.sect[h->macho.n_sects + i].seg_name = (s64 + i)->segname;
			h->macho.sect[h->macho.n_sects + i].offset =
				to_big_endian(h->macho.l_endian, (s64 + i)->offset);
			add_values(h, i, (s64 + i)->addr, (s64 + i)->size);
			i++;
		}
		h->macho.n_sects += sc_nsects;
	}
	return (0);
}

uint8_t	add_sect_32(t_finfo file, t_fhead *h, t_lc *lc)
{
	struct segment_command	*sc;
	struct section			*s32;
	uint32_t				i;
	uint32_t				sc_nsects;

	sc = (struct segment_command *)lc;
	sc_nsects = to_big_endian(h->macho.l_endian, sc->nsects);
	if (sc_nsects > 0)
	{
		i = 0;
		s32 = (struct section *)((char *)sc + sizeof(*sc));
		if (s32 && (char *)sc + sizeof(*s32) > h->ptr + file.size)
			return (1);
		while (i < sc_nsects)
		{
			h->macho.sect[h->macho.n_sects + i].name = (s32 + i)->sectname;
			h->macho.sect[h->macho.n_sects + i].seg_name = (s32 + i)->segname;
			h->macho.sect[h->macho.n_sects + i].offset =
				to_big_endian(h->macho.l_endian, (s32 + i)->offset);
			add_values(h, i, (s32 + i)->addr, (s32 + i)->size);
			i++;
		}
		h->macho.n_sects += sc_nsects;
	}
	return (0);
}
