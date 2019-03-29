/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_nm_otool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:41 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 13:39:26 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIB_NM_OTOOL_H
# define LIB_NM_OTOOL_H

# include "libft.h"
# include <fcntl.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/fat.h>
# include <mach-o/ranlib.h>

typedef struct nlist_64 t_n64;

typedef struct	s_section
{
	char			*name;
	char			*seg_name;
	struct nlist_64	*ptr;
}				t_section;

typedef struct	s_symbols
{
	char		*file_name;
	char		*header_ptr;
	t_section	sections[255];
	uint8_t		n_cmds;
	uint8_t		n_sects;
}				t_symbols;

typedef struct	s_fusion_var
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	k;
	uint32_t	n1;
	uint32_t	n2;
}				t_fus;

uint8_t		handle_architecture(char *arg, char *ptr);

uint8_t		handle_error(char *path);
void		merge_sort(struct nlist_64 *ar[], uint32_t f, uint32_t l);
char		find_symbol_type(t_symbols *sym, t_n64 list);

void		print_struct_sym(t_symbols sym);
void		m_sort(t_n64 *ar[], char *st, uint32_t l, uint32_t r);

#endif
