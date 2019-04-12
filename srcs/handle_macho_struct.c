/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:05:35 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 16:28:40 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	free_error(t_fhead *head)
{
	free(head->macho.arr);
	return (1);
}

uint8_t	copy_32(t_fhead *head, t_sc *sc)
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
			return (free_error(head));
		head->macho.arr[i].nl.n_un.n_strx =
			to_big_endian(head->macho.l_endian, nl->n_un.n_strx);
		head->macho.arr[i].nl.n_type = nl->n_type;
		head->macho.arr[i].nl.n_sect = nl->n_sect;
		head->macho.arr[i].nl.n_desc =
			to_big_endian(head->macho.l_endian, nl->n_desc);
		head->macho.arr[i].nl.n_value =
			to_big_endian(head->macho.l_endian, nl->n_value);
		nl = (void *)nl + sizeof(*nl);
		i++;
	}
	return (0);
}

uint8_t	handle_32(t_finfo file, t_fhead *head, t_sc *sc)
{
	char		*str_tab;

	str_tab = (char *)(head->current
			+ to_big_endian(head->macho.l_endian, sc->stroff));
	if (!(head->macho.arr =
			(t_ulist *)malloc(sizeof(t_ulist) * head->macho.n_syms))
			|| (!str_tab || str_tab > head->ptr + file.size))
		return (1);
	if (copy_32(head, sc))
		return (1);
	if (!(head->opts & OPT_P))
		m_sort(head, str_tab, 0, head->macho.n_syms - 1);
	print_symbols(file, head, str_tab);
	return (0);
}

uint8_t	handle_64(t_finfo file, t_fhead *head, t_sc *sc)
{
	char		*str_tab;
	t_n64		*nl_64;
	uint32_t	i;

	i = 0;
	nl_64 = (t_n64 *)(head->current
			+ to_big_endian(head->macho.l_endian, sc->symoff));
	str_tab = head->current + to_big_endian(head->macho.l_endian, sc->stroff);
	if (!(head->macho.arr =
		(t_ulist *)malloc(sizeof(t_ulist) * head->macho.n_syms))
		|| !(nl_64 && str_tab) || str_tab > head->ptr + file.size)
		return (1);
	while (i < head->macho.n_syms)
	{
		if (!nl_64 || nl_64->n_un.n_strx
				> to_big_endian(head->macho.l_endian, sc->strsize))
			return (free_error(head));
		head->macho.arr[i].nl = *nl_64;
		nl_64 = (void *)nl_64 + sizeof(*nl_64);
		i++;
	}
	if (!(head->opts & OPT_P))
		m_sort(head, str_tab, 0, head->macho.n_syms - 1);
	print_symbols(file, head, str_tab);
	return (0);
}
