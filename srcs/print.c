/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:49:23 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/28 16:43:56 by pchadeni         ###   ########.fr       */
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
