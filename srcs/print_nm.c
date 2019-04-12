/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 16:42:33 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 16:46:09 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static uint8_t	handle_opts(t_fhead *head, t_n64 n)
{
	if ((head->opts & OPT_UP_U) && ((n.n_type & N_TYPE) != N_UNDF
				&& (n.n_type & N_TYPE) != N_PBUD))
		return (1);
	else if ((head->opts & OPT_G) && (n.n_type & N_EXT))
		return (1);
	else if (!(head->opts & OPT_G) && !(head->opts & OPT_U)
			&& !(n.n_type & N_STAB))
		return (1);
	return (0);
}

static void		print_full_sym(t_fhead *h, t_n64 n, char *st)
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

void			print_nm(t_fhead *h, char *st)
{
	uint32_t	i;
	t_n64		symtab;

	i = 0;
	while (i < h->macho.n_syms)
	{
		symtab = h->macho.arr[i].nl;
		if (handle_opts(h, symtab))
			print_full_sym(h, symtab, st);
		else if (((h->opts & OPT_U) && ((symtab.n_type & N_TYPE) == N_UNDF
			|| (symtab.n_type & N_TYPE) == N_PBUD)) || h->opts & OPT_J)
			ft_printf("%s\n", symtab.n_un.n_strx + st);
		i++;
	}
	free(h->macho.arr);
}
