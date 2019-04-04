/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 19:45:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 17:16:16 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static uint8_t	not_in_array(uint32_t *array, uint32_t value, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		if (array[j] == value)
			return (0);
		j++;
	}
	return (1);
}

static uint8_t	foreach_objects(char *arg, char *ptr, uint32_t *arr, uint32_t s)
{
	uint32_t		i;
	char			*object_header;
	struct ar_hdr	*ah;

	i = 0;
	while (i < s)
	{
		ah = (struct ar_hdr *)(ptr + arr[i]);
		object_header = (void *)ah + sizeof(*ah)
			+ ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1));
		ft_printf("\n%s(%s):\n", arg, (void *)ah + sizeof(*ah));
		list_symbols((void *)ah + sizeof(*ah), object_header);
		i++;
	}
	return (0);
}

static uint32_t	*create_array_offset(char *ptr, uint32_t *i)
{
	struct ar_hdr	*ah;
	void			*sz;
	struct ranlib	*ran;
	uint32_t		nb_ranlib;
	uint32_t		*array;

	ah = (struct ar_hdr *)(ptr + SARMAG + ft_strlen(AR_EFMT1));
	sz = (ptr + sizeof(*ah) + SARMAG + ft_atoi(ah->ar_name));
	ran = (struct ranlib *)(sz + sizeof(uint32_t));
	nb_ranlib = *(uint32_t*)sz / sizeof(struct ranlib);
	if (!(array = (uint32_t *)ft_memalloc_uint(sizeof(uint32_t) * nb_ranlib)))
		return (NULL);
	while ((void *)ran < sz + *(uint32_t *)sz)
	{
		if (*i == 0 || not_in_array(array, ran->ran_off, *i))
		{
			array[*i] = ran->ran_off;
			(*i)++;
		}
		ran = (void *)ran + sizeof(struct ranlib);
	}
	return (array);
}

uint8_t			handle_archive(char *arg, char *ptr)
{
	uint32_t	*array_offset;
	uint32_t	i;

	i = 0;
	if (!(array_offset = create_array_offset(ptr, &i)))
		return (1);
	m_sort_uint(array_offset, 0, i - 1);
	list_foreach_objects(arg, ptr, array_offset, i - 1);
	free(array_offset);
	return (0);
}
