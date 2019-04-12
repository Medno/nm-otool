/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_sort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/29 10:43:01 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 16:26:51 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static void	copy_array(t_ulist ar[], t_ulist tmp[], uint32_t sz, uint32_t off)
{
	uint32_t	i;

	i = 0;
	while (i < sz)
	{
		tmp[i] = ar[off + i];
		i++;
	}
}

static void	concat_ar(t_ulist ar[], t_ulist ar_l[], t_ulist ar_r[], t_fus *fus)
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

static void	compare_nl(t_fhead *head, char *st, t_point_32 lm, t_fus fus)
{
	t_ulist	ar_l[fus.n1];
	t_ulist	ar_r[fus.n2];

	copy_array(head->macho.arr, ar_l, fus.n1, lm.x);
	copy_array(head->macho.arr, ar_r, fus.n2, lm.y + 1);
	while (fus.i < fus.n1 && fus.j < fus.n2)
	{
		if (handle_sort(head, ar_l[fus.i], ar_r[fus.j], st))
		{
			head->macho.arr[fus.k] = ar_l[fus.i];
			fus.i++;
		}
		else
		{
			head->macho.arr[fus.k] = ar_r[fus.j];
			fus.j++;
		}
		fus.k++;
	}
	concat_ar(head->macho.arr, ar_l, ar_r, &fus);
}

static void	merge(t_fhead *head, char *st, t_point_32 lm, uint32_t r)
{
	t_fus	fus;

	fus.i = 0;
	fus.j = 0;
	fus.k = lm.x;
	fus.n1 = lm.y - lm.x + 1;
	fus.n2 = r - lm.y;
	compare_nl(head, st, lm, fus);
}

void		m_sort(t_fhead *head, char *st, uint32_t l, uint32_t r)
{
	t_point_32	lm;

	if (l < r)
	{
		lm.x = l;
		lm.y = l + (r - l) / 2;
		m_sort(head, st, lm.x, lm.y);
		m_sort(head, st, lm.y + 1, r);
		merge(head, st, lm, r);
	}
}
