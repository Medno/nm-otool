/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 13:35:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/15 17:19:03 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

char	find_n_type(t_symbols *sym, uint8_t type, uint64_t value, uint8_t sect)
{
	if (type && (type & N_TYPE) == N_UNDF)
	{
		if ((type & N_TYPE) == N_UNDF && value != 0 && type & N_EXT)
			return ('c');
		return ('u');
	}
	else if (type && (type & N_TYPE) == N_ABS)
		return ('a');
	else if (type && (type & N_TYPE) == N_SECT)
	{
		if (ft_strequ(sym->sect[sect - 1].name, SECT_TEXT))
			return ('t');
		else if (ft_strequ(sym->sect[sect - 1].name, SECT_BSS))
			return ('b');
		else if (ft_strequ(sym->sect[sect - 1].name, SECT_DATA))
			return ('d');
		return ('s');
	}
	else if (type && (type & N_TYPE) == N_INDR)
		return ('i');
	return ('?');
}

char	edit_if_ext(char type, uint8_t list_type)
{
	return (list_type & N_EXT ? ft_toupper(type) : type);
}

char	find_sym_type(t_symbols *sym, uint8_t t, uint64_t v, uint8_t s)
{
	char	type;

	type = find_n_type(sym, t, v, s);
	return (edit_if_ext(type, t));
}
