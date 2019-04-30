/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_nm_otool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:41 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/29 17:02:48 by pchadeni         ###   ########.fr       */
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
	char		*name;
	uint8_t		type;
	uint8_t		sect;
	uint64_t	value;
}				t_ulist;

typedef struct	s_section
{
	char			*name;
	char			*seg_name;
	uint32_t		offset;
	uint64_t		size;
	uint64_t		addr;
}				t_section;

typedef struct	s_symbols
{
	char		*obj_name;
	t_section	sect[255];
	uint8_t		n_sects;
	uint32_t	n_syms;
	t_lc		*lc;
	uint8_t		l_endian;
	uint32_t	magic;
	cpu_type_t	cpu_type;
	uint32_t	n_cmds;
	uint32_t	s_lc;
	uint8_t		is64;
	t_ulist		*arr;
	uint8_t		len_value;
	t_mh64		header;
}				t_symbols;

typedef struct	s_finfo
{
	char		*name;
	uint64_t	size;
}				t_finfo;

typedef struct	s_fhead
{
	char		*ptr;
	uint16_t	opts;
	uint8_t		archive;
	uint8_t		fat;
	uint64_t	fat_size;
	char		*fat_arch_ptr;
	t_symbols	macho;
	char		*current;
	char		*fat_arch;
}				t_fhead;

enum			e_opts
{
	OPT_UP_U = (1 << 0),
	OPT_G = (1 << 1),
	OPT_N = (1 << 2),
	OPT_P = (1 << 3),
	OPT_R = (1 << 4),
	OPT_U = (1 << 5),
	OPT_J = (1 << 6),
	FT_NM = (1 << 7),
	FT_OTOOL = (1 << 8),
	OPT_T = (1 << 9),
	OPT_D = (1 << 10),
	OPT_A = (1 << 11),
	MULT = (1 << 12),
	OPT_H = (1 << 13)
};

enum			e_error
{
	E_UNDIF,
	E_UNDIF_FILE,
	E_UNDIF_OPT,
	E_CORRUPT,
	E_NOT_OBJ
};

uint8_t			handle_architecture(char *arg, char *ptr, int s, uint16_t opts);
uint8_t			list_symbols(t_finfo gile, t_fhead *h, char *o_n);
uint8_t			handle_fat(t_finfo file, t_fhead *head, uint32_t magic);
uint8_t			handle_archive(t_finfo gile, t_fhead *head);

uint8_t			handle_file(char *arg, uint8_t res, uint16_t opts);

uint8_t			handle_error(char *path, uint8_t error, uint16_t opts);
uint8_t			invalid_filetype(char *ptr);
int				invalid_parameters(int ac, char **av, uint16_t *o, char *fil[]);

uint8_t			handle_sort(t_fhead *head, t_ulist e_l, t_ulist e_r);

uint8_t			display_mach_header(t_fhead *head);

char			find_sym_type(t_symbols *sy, uint8_t t, uint64_t v, uint8_t s);
char			*cpu_name(cpu_type_t ct, cpu_subtype_t sub);
void			print_symbols(t_finfo f, t_fhead *h);
void			print_nm(t_finfo f, t_fhead *h);
void			print_header_otool(t_finfo file, t_fhead *head);

uint8_t			is_macho(uint32_t magic);
uint8_t			is_fat(uint32_t magic);
uint8_t			is_archive(char *ptr);
uint64_t		to_big_endian(uint8_t l_endian, uint64_t value);

uint8_t			init_symbols_struct(t_finfo file, t_fhead *head, char *name);

uint8_t			free_array(t_fhead *head, uint32_t end);
uint8_t			get_length_value(uint64_t value);
uint8_t			handle_macho(t_finfo file, t_fhead *head, t_sc *sc);
uint8_t			add_sect_64(t_finfo file, t_fhead *head, t_lc *lc);
uint8_t			add_sect_32(t_finfo file, t_fhead *head, t_lc *lc);

void			m_sort(t_fhead *head, uint32_t l, uint32_t r);
void			m_sort_uint(uint32_t *ar, uint32_t l, uint32_t r);
#endif
