/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 19:45:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/03 21:23:08 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint32_t	align(uint32_t size, uint32_t round)
{
	return ((((size - 1) / round) * round) + round);
}

uint8_t	handle_archive(char *arg, char *ptr)
{
	struct ar_hdr	*ah;
	void			*sz;

	(void)arg;
	ah = (struct ar_hdr *)(ptr + SARMAG + ft_strlen(AR_EFMT1));
	sz = (ptr + sizeof(*ah) + SARMAG + ft_atoi(ah->ar_name));
	ft_printf("ptr : |%#x|\n", (ptr));
	ft_printf("ptr : |%#x|\n", sz);
	ft_printf("sz : |%zu|\n", *(uint32_t *)sz);
	return (0);
}
