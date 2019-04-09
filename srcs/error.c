/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:28 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/05 11:44:00 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	handle_error(char *path, uint8_t error)
{
	if (error == E_UNDIF_FILE)
		ft_dprintf(2, "ft_nm: %s: Cannot open file or directory.\n", path);
	else if (error == E_UNDIF_OPT)
		ft_dprintf(2, "ft_nm: Unknown command line argument '%s'. Try ft_nm -help\n", path);

	return (1);
}

uint8_t	invalid_filetype(char *ptr)
{
	uint32_t	magic;

	magic = *(int *)ptr;
	return (!is_macho(magic) && !is_fat(magic) && !is_archive(ptr));
}
