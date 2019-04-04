/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_nm_otool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:41 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 17:21:03 by pchadeni         ###   ########.fr       */
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
# include <ar.h>

typedef struct nlist_64	t_n64;
typedef struct nlist	t_n32;
typedef struct load_command	t_lc;
typedef struct symtab_command	t_sc;
typedef struct mach_header	t_mh;
typedef struct mach_header_64	t_mh64;
typedef struct fat_header	t_fh;
typedef struct fat_arch	t_fa;
typedef struct fat_arch	t_fa64;

typedef struct	s_sort_var
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	k;
	uint32_t	n1;
	uint32_t	n2;
}				t_fus;

typedef struct	s_univ_nlist
{
	t_n64	nl;
}				t_ulist;

typedef struct	s_section
{
	char			*name;
	char			*seg_name;
	struct nlist_64	*ptr;
}				t_section;

typedef struct	s_symbols
{
	char		*file_name;
	uint32_t	magic;
	char		*header_ptr;
	t_section	sections[255];
	uint8_t		n_cmds;
	uint8_t		n_sects;
	uint32_t	n_syms;
	t_lc		*lc;
	uint8_t		l_endian;
}				t_symbols;

uint8_t			handle_architecture(char *arg, char *ptr);
uint8_t			list_symbols(char *arg, void *ptr);

uint8_t			handle_error(char *path);
void			merge_sort(struct nlist_64 *ar[], uint32_t f, uint32_t l);
char			find_sym_type(t_symbols *sy, uint8_t t, uint32_t v, uint8_t s);

void			print_struct_sym(t_symbols sym);
void			m_sort(t_ulist ar[], char *st, uint32_t l, uint32_t r);
void			m_sort_uint(uint32_t *ar, uint32_t l, uint32_t r);
void			print_symbols(t_symbols *sym, t_ulist new[], char *str_tab);

uint8_t			handle_fat(char *arg, char *ptr, uint32_t magic);
uint8_t			handle_archive(char *arg, char *ptr);

uint32_t		convert_little_endian(uint32_t ptr);
uint32_t		to_big_endian(uint8_t l_endian, uint32_t value);
uint8_t			invalid_filetype(char *ptr);

uint8_t			is_macho(uint32_t magic);
uint8_t			is_fat(uint32_t magic);
uint8_t			is_archive(char *ptr);

t_symbols		init_symbols_struct(char *arg, char *ptr);

void			handle_32(t_symbols *sym, t_sc *sc);
void			handle_64(t_symbols *sym, t_sc *sc);
void			add_sect_in_struct_64(t_symbols *sym, t_lc *lc);
void			add_sect_in_struct_32(t_symbols *sym, t_lc *lc);

#endif
