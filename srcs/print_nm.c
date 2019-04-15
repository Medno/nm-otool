/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 16:42:33 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/15 18:08:14 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static uint8_t	opts_display(t_fhead *head, t_ulist n)
{
	if ((head->opts & OPT_UP_U) && ((n.type & N_TYPE) != N_UNDF
				&& (n.type & N_TYPE) != N_PBUD))
		return (1);
	else if ((head->opts & OPT_G) && (n.type & N_EXT))
		return (1);
	else if (!(head->opts & OPT_G) && !(head->opts & OPT_U)
			&& !(n.type & N_STAB))
		return (1);
	return (0);
}

static void		print_full_sym(t_fhead *h, t_ulist n, char *st)
{
	char		type;
	uint8_t		pad;

	(void)st;
	if (h->macho.len_value != 0)
		pad = h->macho.len_value;
	else
		pad = h->macho.is64 ? 16 : 8;
	type = find_sym_type(&h->macho, n.type, n.value, n.sect);
	if (n.value || !n.type || (n.value == 0
				&& (ft_toupper(type) == 'T' || ft_toupper(type) == 'A')))
		ft_printf("%0*llx %c %s\n", pad, n.value, type, n.name);
	else
		ft_printf("%*s %c %s\n", pad, "", type, n.name);
}

void			print_nm(t_fhead *h, char *st)
{
	uint32_t	i;
	t_ulist		symtab;

	i = 0;
	while (i < h->macho.n_syms)
	{
		symtab = h->macho.arr[i];
		if (opts_display(h, symtab))
			print_full_sym(h, symtab, st);
		else if (((h->opts & OPT_U) && ((symtab.type & N_TYPE) == N_UNDF
			|| (symtab.type & N_TYPE) == N_PBUD)) || h->opts & OPT_J)
			ft_printf("%s\n", symtab.name);
		i++;
	}
}
