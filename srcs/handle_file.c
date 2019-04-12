/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 17:50:14 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 17:54:35 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static uint8_t		read_file(char *arg, uint16_t opts, int fd)
{
	char		*ptr;
	struct stat	buf;
	uint8_t		error;

	if (fstat(fd, &buf) != 0)
		error = handle_error(arg, E_UNDIF_FILE, opts);
	else
	{
		if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
				== MAP_FAILED)
			error = handle_error(arg, E_UNDIF_FILE, opts);
		else
		{
			error = handle_architecture(arg, ptr, buf.st_size, opts);
			munmap(ptr, buf.st_size);
		}
	}
	if (close(fd) != 0)
		error = handle_error(arg, E_UNDIF_FILE, opts);
	return (error);
}

uint8_t				handle_file(char *arg, uint8_t res, uint16_t opts)
{
	int			fd;
	uint8_t		error;

	error = 0;
	if ((fd = open(arg, O_RDONLY)) < 0)
		error = handle_error(arg, E_UNDIF, opts);
	else
		error = read_file(arg, opts, fd);
	return (res == 1 || error == 1 ? 1 : 0);
}
