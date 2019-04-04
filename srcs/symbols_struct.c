/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:21:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 16:42:14 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

t_symbols	init_symbols_struct(char *arg, char *ptr)
{
	t_symbols	symbols;
	uint32_t	magic;

	magic = *(int *)ptr;
	symbols.magic = magic;
	symbols.file_name = arg;
	symbols.header_ptr = ptr;
	symbols.n_sects = 0;
	symbols.n_syms = 0;
	symbols.l_endian = magic == MH_MAGIC || magic == MH_MAGIC_64 ? 0 : 1;
	if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		symbols.lc = (t_lc *)((void *)symbols.header_ptr
				+ sizeof(struct mach_header));
		symbols.n_cmds = to_big_endian(symbols.l_endian,
				((t_mh *)(symbols.header_ptr))->ncmds);
	}
	else
	{
		symbols.lc = (t_lc *)(symbols.header_ptr
				+ sizeof(struct mach_header_64));
		symbols.n_cmds = to_big_endian(symbols.l_endian,
				((t_mh64 *)(symbols.header_ptr))->ncmds);
	}
	return (symbols);
}
