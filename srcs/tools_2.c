/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/30 14:54:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/30 14:55:34 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

uint8_t	free_array(t_fhead *head, uint32_t end)
{
	uint32_t	i;

	i = 0;
	while (i < end)
	{
		free(head->macho.arr[i].name);
		i++;
	}
	return (1);
}

uint8_t	get_length_value(uint64_t value)
{
	uint8_t	res;
	char	*itoa_str;

	itoa_str = ft_itoa_base(value, 16);
	res = ft_strlen(itoa_str);
	free(itoa_str);
	return (res);
}
