/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:49:23 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/11 18:28:46 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	print_struct_sym(t_symbols sym)
{
	uint8_t	i;

	ft_printf("===========================================================\n\
			===========================================================\n");
	ft_printf("=======================Print of struct=====================\n");
	ft_printf("Header : |%#x|\n", sym.magic);
	ft_printf("Number of commands : |%zu|\n", sym.n_cmds);
	ft_printf("Number of n_sections : |%zu|\n", sym.n_sects);
	ft_printf("Number of n_syms : |%zu|\n", sym.n_syms);
	ft_printf("Size of n_lc : |%zu|\n", sym.s_lc);
	ft_printf("CPU : |%d|\n", sym.cpu_type);
	ft_printf("endian : |%zu|\n", sym.l_endian);
	if (sym.n_sects > 0)
	{
		i = 0;
		while (i < sym.n_sects)
		{
			ft_printf("Name of section[%zu]: |%s|\n", i, sym.sect[i].name);
			ft_printf("-->	Name of segment: |%s|\n", sym.sect[i].seg_name);
			i++;
		}
	}
	ft_printf("===========================================================\n");
}

void	print_otool(t_finfo f, t_fhead *head, t_ulist new[], char *str_tab)
{
	uint32_t	i;
	t_section	sect;
	uint8_t		sym;
	uint8_t		padding;

	(void)f;
	(void)new;
	(void)str_tab;
	i = 0;
	while (i < head->macho.n_syms)
	{
		if (ft_strequ(head->macho.sect[i].name, SECT_TEXT))
			break ;
		i++;
	}
	sect = head->macho.sect[i];
	i = 0;
	padding = head->macho.is64 ? 16 : 8;
	ft_printf("Contents of (%s,%s) section\n", sect.seg_name, sect.name);
	while (i < sect.size)
	{
		if (i % 16 == 0)
		{
			if (i != 0)
				ft_printf("\n");
			ft_printf("%0*llx	", padding, sect.addr + i);
		}
		sym = *((head->current) + sect.offset + i) < 0
			? *((head->current) + sect.offset + i) << 3 >> 3
			: *((head->current) + sect.offset + i);
		ft_printf("%02x ", sym);
		i++;
	}
}

void	print_symbols(t_finfo f, t_fhead *head, t_ulist new[], char *str_tab)
{
	uint32_t	i;
	char		type;
	uint8_t		padding;

	i = 0;
	padding = head->macho.is64 ? 16 : 8;
	if (f.opts & FT_OTOOL && head->archive == 1 && (head->archive = 2))
		ft_printf("Archive : %s", f.name);
	else if (f.opts & FT_OTOOL)
		ft_printf("%s:\n", f.name);
	if (head->archive && !head->fat_arch)
		ft_printf("\n%s(%s):\n", f.name, head->macho.obj_name);
	else if (head->archive)
		ft_printf("\n%s(%s) (for architecture %s):\n",
				f.name, head->macho.obj_name, head->fat_arch);
	else if (head->fat && head->fat_arch)
		ft_printf("\n%s (for architecture %s):\n",
				f.name, head->fat_arch);
	if ((f.opts & FT_NM))
	{
		while (i < head->macho.n_syms)
		{
			if (!(new[i].nl.n_type & N_STAB))
			{
				type = find_sym_type(&head->macho,
						new[i].nl.n_type, new[i].nl.n_value, new[i].nl.n_sect);
				if (new[i].nl.n_value
						|| (new[i].nl.n_value == 0 && (ft_toupper(type) == 'T'
								|| ft_toupper(type) == 'A')))
					ft_printf("%0*llx %c %s\n", padding, new[i].nl.n_value, type,
							str_tab + new[i].nl.n_un.n_strx);
				else
					ft_printf("%*s %c %s\n", padding, "", type,
							str_tab + new[i].nl.n_un.n_strx);
			}
			i++;
		}
	}
	else
		print_otool(f, head, new, str_tab);
}
