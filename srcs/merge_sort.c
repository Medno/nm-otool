/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 10:43:01 by pchadeni          #+#    #+#             */
/*   Updated: 2019/03/29 13:35:36 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

void	copy_array(t_n64 *ar[], t_n64 *tmp[], uint32_t sz, uint32_t off)
{
	uint32_t	i;

	i = 0;
	while (i < sz)
	{
		tmp[i] = ar[off + i];
		i++;
	}
}

void	concat_array(t_n64 *ar[], t_n64 *ar_l[], t_n64 *ar_r[], t_fus *fus)
{
	while (fus->i < fus->n1)
	{
		ar[fus->k] = ar_l[fus->i];
		(fus->i)++;
		(fus->k)++;
	}
	while (fus->j < fus->n2)
	{
		ar[fus->k] = ar_r[fus->j];
		(fus->j)++;
		(fus->k)++;
	}
}

void	compare_nl(t_n64 *ar[], char *st, t_point_32 lm, t_fus fus)
{
	t_n64	*ar_l[fus.n1];
	t_n64	*ar_r[fus.n2];
	char	*f_str;
	char	*s_str;

	copy_array(ar, ar_l, fus.n1, lm.x);
	copy_array(ar, ar_r, fus.n2, lm.y + 1);
	while (fus.i < fus.n1 && fus.j < fus.n2)
	{
		f_str = st + ar_l[fus.i]->n_un.n_strx;
		s_str = st + ar_r[fus.j]->n_un.n_strx;
		if (ft_strcmp(f_str, s_str) <= 0)
		{
			ar[fus.k] = ar_l[fus.i];
			fus.i++;
		}
		else
		{
			ar[fus.k] = ar_r[fus.j];
			fus.j++;
		}
		fus.k++;
	}
	concat_array(ar, ar_l, ar_r, &fus);
}

void	merge(t_n64 *arr[], char *st, t_point_32 lm, uint32_t r)
{
	t_fus	fus;

	fus.i = 0;
	fus.j = 0;
	fus.k = lm.x;
	fus.n1 = lm.y - lm.x + 1;
	fus.n2 = r - lm.y;
	compare_nl(arr, st, lm, fus);
}

void	m_sort(t_n64 *arr[], char *st, uint32_t l, uint32_t r)
{
	t_point_32	lm;

	if (l < r)
	{
		lm.x = l;
		lm.y = l + (r - l) / 2;
		m_sort(arr, st, lm.x, lm.y);
		m_sort(arr, st, lm.y + 1, r);
		merge(arr, st, lm, r);
	}
}
