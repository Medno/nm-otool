/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort_uint.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/04 14:19:24 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/04 16:45:17 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static void	copy_array(uint32_t *ar, uint32_t tmp[], uint32_t sz, uint32_t off)
{
	uint32_t	i;

	i = 0;
	while (i < sz)
	{
		tmp[i] = ar[off + i];
		i++;
	}
}

static void	concat_ar(uint32_t *ar, uint32_t ar_l[], uint32_t ar_r[], t_fus *f)
{
	while (f->i < f->n1)
	{
		ar[f->k] = ar_l[f->i];
		(f->i)++;
		(f->k)++;
	}
	while (f->j < f->n2)
	{
		ar[f->k] = ar_r[f->j];
		(f->j)++;
		(f->k)++;
	}
}

static void	compare_nl(uint32_t *ar, t_point_32 lm, t_fus fus)
{
	uint32_t	ar_l[fus.n1];
	uint32_t	ar_r[fus.n2];

	copy_array(ar, ar_l, fus.n1, lm.x);
	copy_array(ar, ar_r, fus.n2, lm.y + 1);
	while (fus.i < fus.n1 && fus.j < fus.n2)
	{
		if (ar_l[fus.i] <= ar_r[fus.j])
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
	concat_ar(ar, ar_l, ar_r, &fus);
}

static void	merge(uint32_t *arr, t_point_32 lm, uint32_t r)
{
	t_fus	fus;

	fus.i = 0;
	fus.j = 0;
	fus.k = lm.x;
	fus.n1 = lm.y - lm.x + 1;
	fus.n2 = r - lm.y;
	compare_nl(arr, lm, fus);
}

void		m_sort_uint(uint32_t *arr, uint32_t l, uint32_t r)
{
	t_point_32	lm;

	if (l < r)
	{
		lm.x = l;
		lm.y = l + (r - l) / 2;
		m_sort_uint(arr, lm.x, lm.y);
		m_sort_uint(arr, lm.y + 1, r);
		merge(arr, lm, r);
	}
}
