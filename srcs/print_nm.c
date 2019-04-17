/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 16:42:33 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/17 15:26:52 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static uint8_t	opts_display(t_fhead *head, t_ulist n, char type)
{
	if (n.type & N_STAB)
		return (0);
	if (head->opts & OPT_U)
	{
		if (type != 'U')
			return (0);
	}
	if (head->opts & OPT_UP_U)
	{
		if (type == 'U')
			return (0);
	}
	if (head->opts & OPT_G)
		if (ft_toupper(type) != type)
			return (0);
	return (1);

}

static void		print_full_sym(t_fhead *h, t_ulist n, char type)
{
	uint8_t		pad;

	if (h->macho.len_value != 0)
		pad = h->macho.len_value;
	else
		pad = h->macho.is64 ? 16 : 8;
	if (n.value || !n.type || (n.value == 0
				&& (ft_toupper(type) == 'T' || ft_toupper(type) == 'A')))
		ft_printf("%0*llx %c %s\n", pad, n.value, type, n.name);
	else
		ft_printf("%*s %c %s\n", pad, "", type, n.name);
}

static void		display_file_name(t_finfo file, t_fhead *head)
{
	if (head->archive && !head->fat_arch)
		ft_printf("%s:%s: ", file.name, head->macho.obj_name);
	else if (head->archive)
		ft_printf("(for architecture %s):%s:%s: ",
				head->fat_arch, file.name, head->macho.obj_name);
	else if (head->fat && head->fat_arch)
		ft_printf("(for architecture %s):%s: ", head->fat_arch, file.name);
	else
		ft_printf("%s: ", file.name);
}

void			print_nm(t_finfo f, t_fhead *h)
{
	uint32_t	i;
	t_ulist		symtab;
	char		type;

	i = 0;
	while (i < h->macho.n_syms)
	{
		symtab = h->macho.arr[i];
		type = find_sym_type(&h->macho, symtab.type, symtab.value, symtab.sect);
		if (opts_display(h, symtab, type))
		{
			if (h->opts & OPT_A)
				display_file_name(f, h);
			if (!(h->opts & OPT_J))
				print_full_sym(h, symtab, type);
			else if (h->opts & OPT_J)
				ft_printf("%s\n", symtab.name);
		}
		i++;
	}
}
