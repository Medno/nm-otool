/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_fat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:18:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/03 16:20:20 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	handle_fat(char *ptr, uint32_t magic)
{
	if (magic == FAT_MAGIC)
		ft_printf("32bits FAT file, I'm not treated yet\n");
	else if (magic == FAT_MAGIC_64)
		ft_printf("64bits FAT file, I'm not treated yet\n");
	else if (magic == FAT_CIGAM)
	{
		ft_printf("32bits little endian file, development in progress...\n");
		t_fh	*fh;
		fh = (t_fh *)ptr;
		//		uint32_t	n_fa;
		ft_printf("Number of arch : |%zu|	Converted : |%zu|\n", fh->nfat_arch,
			convert_little_endian(fh->nfat_arch));
//		n_fa = fh->nfat_arch;
		}
		else if (magic == FAT_CIGAM_64)
			ft_printf("64bits little endian file, I'm not treated yet\n");
	return (0);
}
