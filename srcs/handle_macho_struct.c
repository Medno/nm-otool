/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_macho_struct.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 17:05:35 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/30 16:39:02 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	dup_stab_name(t_fhead *head, uint8_t bs, char *str, uint32_t i)
{
	if (bs)
		head->macho.arr[i].name = ft_strdup("bad string index");
	else
		head->macho.arr[i].name = ft_strdup(str);
	if (!head->macho.arr[i].name)
		return (1);
	return (0);
}

uint8_t	handle_32(t_fhead *head, char *str_tab, uint32_t strsize, uint32_t soff)
{
	t_n32		*nl;
	uint32_t	i;
	uint32_t	strx;

	i = 0;
	nl = (t_n32 *)(head->current + soff);
	while (i < head->macho.n_syms)
	{
		strx = to_big_endian(head->macho.l_endian, nl->n_un.n_strx);
		if (strx > strsize && head->opts & FT_OTOOL)
			return (free_array(head, i));
		else if (dup_stab_name(head, strx > strsize, strx + str_tab, i))
			return (free_array(head, i));
		head->macho.arr[i].type = nl->n_type;
		head->macho.arr[i].sect = nl->n_sect;
		head->macho.arr[i].value =
			to_big_endian(head->macho.l_endian, nl->n_value);
		if (head->macho.arr[i].value > 0xffffffff)
			head->macho.len_value = get_length_value(head->macho.arr[i].value);
		nl = (void *)nl + sizeof(*nl);
		i++;
	}
	return (0);
}

uint8_t	handle_64(t_fhead *head, char *str_tab, uint32_t strsize, uint32_t soff)
{
	t_n64		*nl_64;
	uint32_t	i;
	uint32_t	strx;

	i = 0;
	nl_64 = (t_n64 *)(head->current + soff);
	while (i < head->macho.n_syms)
	{
		strx = to_big_endian(head->macho.l_endian, nl_64->n_un.n_strx);
		if (strx > strsize && (head->opts & FT_OTOOL) && !(head->opts & OPT_H))
			return (free_array(head, i));
		else if (dup_stab_name(head, strx > strsize, strx + str_tab, i))
			return (free_array(head, i));
		head->macho.arr[i].type = nl_64->n_type;
		head->macho.arr[i].sect = nl_64->n_sect;
		head->macho.arr[i].value =
			to_big_endian(head->macho.l_endian, nl_64->n_value);
		if (head->macho.arr[i].value > 0xfffffffffffffff)
			head->macho.len_value = get_length_value(head->macho.arr[i].value);
		nl_64 = (void *)nl_64 + sizeof(*nl_64);
		i++;
	}
	return (0);
}

uint8_t	handle_macho(t_finfo file, t_fhead *head, t_sc *sc)
{
	char		*str_tab;
	uint32_t	strsize;
	uint32_t	symoff;
	uint8_t		res;

	str_tab = head->current + to_big_endian(head->macho.l_endian, sc->stroff);
	if (str_tab > head->ptr + file.size || !(head->macho.arr =
				(t_ulist *)malloc(sizeof(t_ulist) * head->macho.n_syms)))
		return (1);
	strsize = to_big_endian(head->macho.l_endian, sc->strsize);
	symoff = to_big_endian(head->macho.l_endian, sc->symoff);
	res = head->macho.is64 ? handle_64(head, str_tab, strsize, symoff)
		: handle_32(head, str_tab, strsize, symoff);
	if (!res)
	{
		m_sort(head, 0, head->macho.n_syms - 1);
		print_symbols(file, head);
		free_array(head, head->macho.n_syms);
	}
	else
		ft_printf("%s:\n", file.name);
	free(head->macho.arr);
	return (res);
}
