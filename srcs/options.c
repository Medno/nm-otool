/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 17:12:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/30 16:44:49 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

static int	handle_nm_opts(char t, uint16_t *opts)
{
	if (t == 'u' && !(*opts & OPT_U))
		*opts = *opts ^ OPT_U;
	else if (t == 'g' && !(*opts & OPT_G))
		*opts = *opts ^ OPT_G;
	else if (t == 'n' && !(*opts & OPT_N))
		*opts = *opts ^ OPT_N;
	else if (t == 'p' && !(*opts & OPT_P))
		*opts = *opts ^ OPT_P;
	else if (t == 'r' && !(*opts & OPT_R))
		*opts = *opts ^ OPT_R;
	else if (t == 'U' && !(*opts & OPT_UP_U))
		*opts = *opts ^ OPT_UP_U;
	else if (t == 'j' && !(*opts & OPT_J))
		*opts = *opts ^ OPT_J;
	else if (t == 'A' && !(*opts & OPT_A))
		*opts = *opts ^ OPT_A;
	else
		return (1);
	return (0);
}

int			handle_opts(char *str, uint16_t *opts, size_t size)
{
	size_t	i;

	i = 1;
	while (i < size)
	{
		if (*opts & FT_NM && handle_nm_opts(str[i], opts))
			return (1);
		else if (*opts & FT_OTOOL)
		{
			if (str[i] == 't')
				*opts = *opts | OPT_T;
			else if (str[i] == 'd')
				*opts = *opts | OPT_D;
			else if (str[i] == 'h')
				*opts = *opts | OPT_H;
			else
				return (1);
		}
		i++;
	}
	return (0);
}

int			invalid_parameters(int ac, char **av, uint16_t *opts, char *files[])
{
	int		i;
	int		nb_files;
	uint8_t	end_of_opts;

	i = 1;
	nb_files = 0;
	end_of_opts = 0;
	while (i < ac)
	{
		if (av[i][0] == '-' && !end_of_opts)
		{
			if (ft_strequ("--", av[i]))
				end_of_opts = 1;
			else if (handle_opts(av[i], opts, ft_strlen(av[i])))
			{
				files[0] = av[i];
				return (-1);
			}
		}
		else
			files[nb_files++] = av[i];
		i++;
	}
	return (nb_files);
}

uint8_t		handle_sort(t_fhead *head, t_ulist e_l, t_ulist e_r)
{
	uint8_t	condition;
	char	*f_str;
	char	*s_str;

	f_str = e_l.name;
	s_str = e_r.name;
	if (head->opts & OPT_N)
		condition = ((!e_l.value && !e_r.value && ft_strcmp(f_str, s_str) < 0
			&& e_l.type == e_r.type) || (!e_l.value && e_l.type == 1)
			|| (!e_l.value && e_r.value)
			|| (e_l.value && e_r.value && e_l.value < e_r.value)
			|| (e_l.value && e_l.value == e_r.value
				&& ft_strcmp(f_str, s_str) < 0));
	else
		condition = ft_strcmp(f_str, s_str) < 0
			|| (ft_strequ(f_str, s_str) && e_l.value < e_r.value);
	if (head->opts & OPT_R)
		condition = !condition;
	return (condition);
}
