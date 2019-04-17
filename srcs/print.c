/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:49:23 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/17 14:58:59 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static t_section	*find_sect(t_fhead *head)
{
	uint32_t	i;

	i = 0;
	while (i < head->macho.n_sects
			&& ((!ft_strequ(head->macho.sect[i].name, SECT_TEXT)
			&& head->opts & OPT_T) || (head->opts & OPT_D
			&& !ft_strequ(head->macho.sect[i].name, SECT_DATA))))
		i++;
	if (i == head->macho.n_sects)
		return (NULL);
	return (&head->macho.sect[i]);
}

static void			print_otool(t_fhead *head, t_section *sect, uint8_t padding)
{
	uint32_t	i;
	uint8_t		sym;
	uint8_t		space;

	i = 0;
	space = head->macho.cpu_type == CPU_TYPE_X86_64
		|| head->macho.cpu_type == CPU_TYPE_I386;
	ft_printf("Contents of (%s,%s) section", sect->seg_name, sect->name);
	while (i < sect->size)
	{
		if (i % 16 == 0)
			ft_putchar('\n');
		if (i % 16 == 0)
			ft_printf("%0*llx\t", padding, (sect->addr + i));
		sym = *((head->current) + sect->offset + i) < 0
			? *((head->current) + sect->offset + i) << 3 >> 3
			: *((head->current) + sect->offset + i);
		ft_printf("%02x", sym);
		if (space)
			ft_putchar(' ');
		i++;
		if (!space && i % 4 == 0 && i)
			ft_putchar(' ');
	}
}

static void			print_header(t_finfo f, t_fhead *head)
{
	if (head->opts & FT_NM && ((head->archive || (head->fat && head->fat_arch))
		|| (head->opts & MULT)))
		ft_putchar('\n');
	if (head->opts & FT_OTOOL && head->archive == 1 && (head->archive = 2))
		ft_printf("Archive : %s\n", f.name);
	else if (head->opts & FT_OTOOL && head->fat && head->fat_arch)
		ft_printf("%s (architecture %s):\n", f.name, head->fat_arch);
	else if (head->opts & FT_OTOOL && !head->archive)
		ft_printf("%s:\n", f.name);
	if (head->archive && !head->fat_arch)
		ft_printf("%s(%s):\n", f.name, head->macho.obj_name);
	else if (head->archive)
		ft_printf("%s(%s) (for architecture %s):\n",
				f.name, head->macho.obj_name, head->fat_arch);
	else if (head->fat && head->fat_arch && head->opts & FT_NM)
		ft_printf("%s (for architecture %s):\n", f.name, head->fat_arch);
	else if (head->opts & MULT)
		ft_printf("%s:\n", f.name);
}

void				print_symbols(t_finfo f, t_fhead *h)
{
	uint8_t		padding;
	t_section	*sect;

	padding = h->macho.is64 ? 16 : 8;
	if (!(h->opts & OPT_A))
		print_header(f, h);
	if ((h->opts & FT_NM) && !((h->opts & OPT_U) && (h->opts & OPT_UP_U)))
		print_nm(f, h);
	else if (h->opts & FT_OTOOL)
	{
		sect = find_sect(h);
		if (!sect)
			return ;
		print_otool(h, sect, padding);
		ft_putchar('\n');
	}
}
