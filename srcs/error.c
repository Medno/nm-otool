/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:28 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 16:40:56 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	handle_error(char *path)
{
	ft_dprintf(2, "nm: %s: Cannot open file or directory.\n", path);
	return (1);
}

uint8_t	invalid_filetype(char *ptr)
{
	uint32_t	magic;

	magic = *(int *)ptr;
	ft_printf("Magic number : |%#x|\n", magic);
	return (!is_macho(magic) && !is_fat(magic) && !is_archive(ptr));
}
