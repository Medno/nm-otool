/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 13:35:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 13:38:16 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

char	find_n_type(t_symbols *sym, struct nlist_64 list)
{
	if (list.n_type & N_TYPE)
	{
		if ((list.n_type & N_TYPE) == N_UNDF
				|| (list.n_type & N_TYPE) == N_PBUD)
		{
			if ((list.n_type & N_TYPE) == N_UNDF && list.n_value != 0)
				return ('c');
			return ('u');
		}
		else if ((list.n_type & N_TYPE) == N_ABS)
			return ('a');
		else if ((list.n_type & N_TYPE) == N_SECT)
		{
			if (ft_strequ(sym->sections[list.n_sect - 1].seg_name, SEG_TEXT))
				return ('t');
			if (ft_strequ(sym->sections[list.n_sect - 1].seg_name, SEG_DATA))
				return ('d');
			if (ft_strequ(sym->sections[list.n_sect - 1].name, SECT_BSS))
				return ('b');
			return ('?');
		}
		else if ((list.n_type & N_TYPE) == N_INDR)
			return ('i');
	}
	return ('u');
}

char	edit_if_ext(char type, uint8_t list_type)
{
	return (list_type & N_EXT ? ft_toupper(type) : type);
}

char	find_symbol_type(t_symbols *sym, t_n64 list)
{
	char	type;

	type = find_n_type(sym, list);
	return (edit_if_ext(type, list.n_type));
}
