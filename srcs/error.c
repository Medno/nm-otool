/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:28 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/03 16:14:16 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	handle_error(char *path)
{
	ft_dprintf(2, "nm: %s: Cannot open file or directory.\n", path);
	return (1);
}

uint8_t		invalid_filetype(char *ptr, uint32_t magic)
{
	char		**tmp;

	ft_printf("Magic number : |%#x|\n", magic);
	if (magic != MH_MAGIC && magic != MH_MAGIC_64
			&& magic != MH_CIGAM && magic != MH_CIGAM_64
			&& magic != FAT_MAGIC_64 && magic != FAT_MAGIC
			&& magic != FAT_CIGAM_64 && magic != FAT_CIGAM)
	{
		tmp = ft_splitwsp(ptr);
		if (tmp && tmp[0])
		{
			if (!ft_strequ(tmp[0], "!<arch>"))
				return (1);
			ft_printf("I'm an archive\n");
		}
	}
	return (0);
}
