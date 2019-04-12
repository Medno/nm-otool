/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:49:23 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 15:30:05 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static t_section	find_sect(t_fhead *head)
{
	uint32_t	i;

	i = 0;
	while (i < head->macho.n_syms
			&& !ft_strequ(head->macho.sect[i].name, SECT_TEXT))
		i++;
	return (head->macho.sect[i]);
}

static void			print_otool(t_fhead *head, t_section sect, uint8_t padding)
{
	uint32_t	i;
	uint8_t		sym;
	uint8_t		space;

	i = 0;
	space = head->macho.cpu_type == CPU_TYPE_X86_64
		|| head->macho.cpu_type == CPU_TYPE_I386;
	ft_printf("Contents of (%s,%s) section\n", sect.seg_name, sect.name);
	while (i < sect.size)
	{
		if (i % 16 == 0 && i)
			ft_putchar('\n');
		if (i % 16 == 0)
			ft_printf("%0*llx\t", padding, (sect.addr + i));
		sym = *((head->current) + sect.offset + i) < 0
			? *((head->current) + sect.offset + i) << 3 >> 3
			: *((head->current) + sect.offset + i);
		ft_printf("%02x", sym);
		if (space)
			ft_putchar(' ');
		i++;
		if (!space && i % 4 == 0 && i)
			ft_putchar(' ');
	}
}

static uint8_t		handle_opts(t_finfo file, t_fhead *head, t_n64 n)
{
	(void) head;

	if ((file.opts & OPT_UP_U) && ((n.n_type & N_TYPE) != N_UNDF
				&& (n.n_type & N_TYPE) != N_PBUD))
		return (1);
	if ((file.opts & OPT_G) && (n.n_type & N_EXT))
		return (1);
	else if (file.opts == FT_NM && !(n.n_type & N_STAB))
		return (1);
	return (0);
}

static void			print_full_sym(t_fhead *h, t_n64 n, char *st)
{
	char		type;
	uint8_t		pad;

	pad = h->macho.is64 ? 16 : 8;
	type = find_sym_type(&h->macho, n.n_type, n.n_value, n.n_sect);
	if (n.n_value || (n.n_value == 0
		&& (ft_toupper(type) == 'T' || ft_toupper(type) == 'A')))
		ft_printf("%0*llx %c %s\n", pad, n.n_value, type, st + n.n_un.n_strx);
	else
		ft_printf("%*s %c %s\n", pad, "", type, st + n.n_un.n_strx);
}

static void			print_nm(t_finfo file, t_fhead *h, char *st)
{
	uint32_t	i;
	t_n64		symtab;

	i = 0;
	while (i < h->macho.n_syms)
	{
		symtab = h->macho.arr[i].nl;
		if (handle_opts(file, h, symtab))
			print_full_sym(h, symtab, st);
		else if (((file.opts & OPT_U) && ((symtab.n_type & N_TYPE) == N_UNDF
				|| (symtab.n_type & N_TYPE) == N_PBUD)) || file.opts & OPT_J)
			ft_printf("%s\n", symtab.n_un.n_strx + st);
		i++;
	}
}

static void			print_header(t_finfo f, t_fhead *head)
{
	if (f.opts & FT_NM && (head->archive || (head->fat && head->fat_arch)))
		ft_putchar('\n');
	if (f.opts & FT_OTOOL && head->archive == 1 && (head->archive = 2))
		ft_printf("Archive : %s\n", f.name);
	else if (head->fat && head->fat_arch)
		ft_printf("%s (architecture %s):\n", f.name, head->fat_arch);
	else if (f.opts & FT_OTOOL && !head->archive)
		ft_printf("%s:\n", f.name);
	if (head->archive && !head->fat_arch)
		ft_printf("%s(%s):\n", f.name, head->macho.obj_name);
	else if (head->archive)
		ft_printf("%s(%s) (for architecture %s):\n",
				f.name, head->macho.obj_name, head->fat_arch);
	else if (head->fat && head->fat_arch && f.opts & FT_NM)
		ft_printf("%s (for architecture %s):\n", f.name, head->fat_arch);
}

void				print_symbols(t_finfo f, t_fhead *h, char *st)
{
	uint8_t		padding;

	padding = h->macho.is64 ? 16 : 8;
	print_header(f, h);
	if ((f.opts & FT_NM))
		print_nm(f, h, st);
	else
	{
		print_otool(h, find_sect(h), padding);
		ft_putchar('\n');
	}
}
