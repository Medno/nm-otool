/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_archive.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 19:45:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/09 19:11:20 by pchadeni         ###   ########.fr       */
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

static char		*find_correct_size(struct ar_hdr *ah)
{
	if (ft_strnequ(ah->ar_name, AR_EFMT1, 3))
		return ((void *)ah + sizeof(*ah)
				+ ft_atoi(ah->ar_name + ft_strlen(AR_EFMT1)));
	return ((void *)ah + sizeof(*ah));
}

static uint8_t	for_obj(t_finfo f, t_fhead *head, uint32_t *arr, uint32_t s)
{
	uint32_t		i;
	char			*object_header;
	struct ar_hdr	*ah;
	char			*obj_name;
	char			*ptr;

	i = 0;
	ptr = head->current;
	while (i < s)
	{
		ah = (struct ar_hdr *)(ptr + arr[i]);
		if (ptr + arr[i] > ptr + f.size)
			return (1);
		object_header = find_correct_size(ah);
//		fullpath = ft_strjoin(arg, (void *)ah + sizeof(*ah));
//		ft_printf("\n%s(%s):\n", arg, (void *)ah + sizeof(*ah));
//		list_symbols(fullpath, object_header);
		obj_name = (ft_strnequ(AR_EFMT1, ah->ar_name, 3))
			? (void *)ah + sizeof(*ah) : ah->ar_name;
		head->current = object_header;
		list_symbols(f, head, obj_name);
//		free(fullpath);
		i++;
	}
	return (0);
}

static uint32_t	*create_array_off(t_finfo file, t_fhead *head, uint32_t *i)
{
	struct ar_hdr	*ah;
	char			*n_elem;
	struct ranlib	*ran;
	uint32_t		nb_ranlib;
	uint32_t		*array;

	if (head->current + SARMAG > head->current + file.size)
		return (NULL);
	ah = (struct ar_hdr *)(head->current + SARMAG);
	n_elem = find_correct_size(ah);
	ran = (struct ranlib *)(n_elem + sizeof(uint32_t));
	nb_ranlib = *(uint32_t*)n_elem / sizeof(struct ranlib);
	if (!(array = (uint32_t *)ft_memalloc_uint(nb_ranlib)))
		return (NULL);
	while ((char *)ran < n_elem + *(uint32_t *)n_elem)
	{
		if ((char *)ran + sizeof(*ran) > head->current + file.size)
			return (NULL);
		if (*i == 0 || not_in_array(array, ran->ran_off, *i))
		{
			array[*i] = ran->ran_off;
			(*i)++;
		}
		ran = (void *)ran + sizeof(struct ranlib);
	}
	return (array);
}

uint8_t			handle_archive(t_finfo file, t_fhead *head)
{
	uint32_t	*array_offset;
	uint32_t	i;
	uint8_t		res;

	i = 0;
	head->archive = 1;
	if (!(array_offset = create_array_off(file, head, &i)))
		return (1);
	m_sort_uint(array_offset, 0, i - 1);
	res = for_obj(file, head, array_offset, i);
	free(array_offset);
	return (res);
}
