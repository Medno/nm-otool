/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:49:23 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/01 18:49:50 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	print_struct_sym(t_symbols sym)
{
	uint8_t	i;

	ft_printf("===========================================================\n\
===========================================================\n");
	ft_printf("=======================Print of struct=====================\n");
	ft_printf("Filename : |%s|\n", sym.file_name);
	ft_printf("Header_ptr : |%x|\n", sym.header_ptr);
	ft_printf("Number of commands : |%zu|\n", sym.n_cmds);
	ft_printf("Number of n_sections : |%zu|\n", sym.n_sects);
	ft_printf("Number of n_syms : |%zu|\n", sym.n_syms);
	if (sym.n_sects > 0)
	{
		i = 0;
		while (i < sym.n_sects)
		{
			ft_printf("Name of section[%zu]: |%s|\n", i, sym.sections[i].name);
			ft_printf("-->	Name of segment: |%s|\n", sym.sections[i].seg_name);
			i++;
		}
	}
}

void	print_symbols(t_symbols *sym, t_ulist new[], char *str_tab)
{
	uint32_t	i;
	char		type;
	uint8_t		padding;

	i = 0;
	padding = sym->magic == MH_MAGIC || sym->magic == MH_CIGAM
		? 8 : 16 ;
	while (i < sym->n_syms)
	{
		type = find_symbol_type(sym, new[i].nl.n_type, new[i].nl.n_value,
				new[i].nl.n_sect);
		if (new[i].nl.n_value)
			ft_printf("%0*llx %c %s\n", padding, new[i].nl.n_value, type,
					str_tab + new[i].nl.n_un.n_strx);
		else
			ft_printf("%*s %c %s\n", padding, "", type,
					str_tab + new[i].nl.n_un.n_strx);
		i++;
	}

}
