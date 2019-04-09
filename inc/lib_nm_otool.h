/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_nm_otool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:41 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/09 19:14:48 by pchadeni         ###   ########.fr       */
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
# include <mach-o/arch.h>
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
	char		*obj_name;
	uint32_t	magic;
	t_section	sections[255];
	uint8_t		n_cmds;
	uint8_t		n_sects;
	uint32_t	n_syms;
	t_lc		*lc;
	uint8_t		l_endian;
}				t_symbols;

typedef struct	s_finfo
{
	char				*name;
	int					size;
	uint8_t				opts;
	const NXArchInfo	*arch;
}				t_finfo;

typedef struct	s_fhead
{
	char		*ptr;
	uint8_t		archive;
	uint8_t		fat;
	t_symbols	macho;
	char		*current;
}				t_fhead;

enum			e_opts
{
	OPT_A = (1 << 0),
	OPT_G = (1 << 1),
	OPT_N = (1 << 2),
	OPT_P = (1 << 3),
	OPT_R = (1 << 4),
	OPT_U = (1 << 5),
};

enum			e_error
{
	E_UNDIF_FILE,
	E_UNDIF_OPT
};


uint8_t			handle_architecture(char *arg, char *ptr, int s, uint8_t opts);
uint8_t			list_symbols(t_finfo gile, t_fhead *h, char *o_n);

uint8_t			handle_error(char *path, uint8_t error);
void			merge_sort(struct nlist_64 *ar[], uint32_t f, uint32_t l);
char			find_sym_type(t_symbols *sy, uint8_t t, uint32_t v, uint8_t s);

void			print_struct_sym(t_symbols sym);
void			m_sort(t_ulist ar[], char *st, uint32_t l, uint32_t r);
void			m_sort_uint(uint32_t *ar, uint32_t l, uint32_t r);
void			print_symbols(t_finfo f, t_fhead *h, t_ulist n[], char *s_tab);

uint8_t			handle_fat(t_finfo file, t_fhead *head, uint32_t magic);
uint8_t			handle_archive(t_finfo gile, t_fhead *head);

uint32_t		convert_little_endian(uint32_t ptr);
uint32_t		to_big_endian(uint8_t l_endian, uint32_t value);
uint8_t			invalid_filetype(char *ptr);

uint8_t			is_macho(uint32_t magic);
uint8_t			is_fat(uint32_t magic);
uint8_t			is_archive(char *ptr);

t_symbols		init_symbols_struct(char *name, void *ptr);

void			handle_32(t_finfo file, t_fhead *head, t_sc *sc);
void			handle_64(t_finfo file, t_fhead *head, t_sc *sc);
void			add_sect_in_struct_64(t_symbols *sym, t_lc *lc);
void			add_sect_in_struct_32(t_symbols *sym, t_lc *lc);

#endif
