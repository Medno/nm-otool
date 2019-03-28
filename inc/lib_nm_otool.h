/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib_nm_otool.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:41 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/27 17:49:42 by pchadeni         ###   ########.fr       */
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

uint8_t		handle_architecture(char *arg, char *ptr);

uint8_t		handle_error(char *path);
#endif
