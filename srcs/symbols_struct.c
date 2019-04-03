/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:21:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/03 16:22:35 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

t_symbols	init_symbols_struct(char *arg, char *ptr)
{
	t_symbols	symbols;

	symbols.magic = *(int *)ptr;
	symbols.file_name = arg;
	symbols.header_ptr = ptr;
	symbols.n_sects = 0;
	symbols.n_syms = 0;
	if (symbols.magic == MH_MAGIC)
	{
		symbols.lc = (t_lc *)((void *)symbols.header_ptr
				+ sizeof(struct mach_header));
		symbols.n_cmds = ((struct mach_header *)(symbols.header_ptr))->ncmds;
	}
	else
	{
		symbols.lc = (t_lc *)(symbols.header_ptr
				+ sizeof(struct mach_header_64));
		symbols.n_cmds = ((struct mach_header_64 *)(symbols.header_ptr))->ncmds;
	}
	return (symbols);
}
