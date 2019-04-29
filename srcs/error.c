/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:28 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/29 16:46:07 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	handle_error(char *path, uint8_t error, uint16_t opts)
{
	if (opts & FT_NM)
		ft_dprintf(2, "ft_nm: ");
	if (error == E_UNDIF)
		ft_dprintf(2, "%s: No such file or directory.\n", path);
	else if (error == E_UNDIF_FILE)
		ft_dprintf(2, "%s: Cannot open file or directory.\n", path);
	else if (error == E_NOT_OBJ && (opts & FT_NM))
		ft_dprintf(2,
			"%s: The file was not recognized as a valid object file\n", path);
	else if (error == E_NOT_OBJ && (opts & FT_OTOOL))
		ft_printf("%s: is not an object file\n", path);
	else if (error == E_UNDIF_OPT)
		ft_dprintf(2,
		"Unknown command line argument '%s'. Try ft_nm -help\n", path);
	else if (error == E_CORRUPT && (opts & FT_OTOOL))
		ft_dprintf(2, "ft_otool: %s truncated or malformed file\n", path);
	else if (error == E_CORRUPT)
		ft_dprintf(2, "%s truncated or malformed file\n", path);
	return (1);
}

uint8_t	invalid_filetype(char *ptr)
{
	uint32_t	magic;

	magic = *(int *)ptr;
	return (!is_macho(magic) && !is_fat(magic) && !is_archive(ptr));
}
