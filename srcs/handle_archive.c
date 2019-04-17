/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 19:45:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/17 11:48:20 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static char		*find_correct_size(struct ar_hdr *ah)
{
	int	off;

	off = (ft_strnequ(ah->ar_name, AR_EFMT1, 3))
		? ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1))
		: 0;
	return ((void *)ah + sizeof(*ah) + off);
}

static uint8_t	for_obj(t_finfo f, t_fhead *head, char *first)
{
	uint32_t		i;
	char			*object_header;
	struct ar_hdr	*ah;
	char			*obj_name;
	char			*ptr;

	i = 0;
	ptr = head->current;
	ah = (struct ar_hdr *)(first);
	while (((char *)ah < ptr + f.size && !head->fat)
		|| (head->fat && (char *)ah < head->fat_arch_ptr + head->fat_size))
	{
		if ((char*)ah > ptr + f.size)
			return (handle_error(f.name, E_CORRUPT, head->opts));
		object_header = find_correct_size(ah);
		obj_name = (ft_strnequ(AR_EFMT1, ah->ar_name, 3))
			? (void *)ah + sizeof(*ah) : ah->ar_name;
		head->current = object_header;
		if (list_symbols(f, head, obj_name))
			return (1);
		ah = (void *)ah + sizeof(struct ar_hdr) + ft_atoi(ah->ar_size);
	}
	return (0);
}

uint8_t			handle_archive(t_finfo file, t_fhead *head)
{
	uint8_t	res;
	char	*symtab_size;
	char	*strtab_size;
	char	*first;

	head->archive = 1;
	if (head->current + SARMAG > head->current + file.size)
		return (1);
	symtab_size = find_correct_size((struct ar_hdr *)(head->current + SARMAG));
	strtab_size = *(uint32_t *)symtab_size + symtab_size + sizeof(uint32_t);
	first = strtab_size + sizeof(uint32_t) + *(uint32_t *)strtab_size;
	if (head->current + SARMAG > head->current + file.size)
		return (1);
	res = for_obj(file, head, first);
	return (res);
}
