/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 17:12:57 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 16:48:15 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

int		handle_opts(char *str, uint16_t *opts, size_t size)
{
	size_t	i;

	i = 1;
	while (i < size)
	{
		if (str[i] == 'u' && !(*opts & OPT_U))
			*opts = *opts ^ OPT_U;
		else if (str[i] == 'g' && !(*opts & OPT_G))
			*opts = *opts ^ OPT_G;
		else if (str[i] == 'n' && !(*opts & OPT_N))
			*opts = *opts ^ OPT_N;
		else if (str[i] == 'p' && !(*opts & OPT_P))
			*opts = *opts ^ OPT_P;
		else if (str[i] == 'r' && !(*opts & OPT_R))
			*opts = *opts ^ OPT_R;
		else if (str[i] == 'U' && !(*opts & OPT_UP_U))
			*opts = *opts ^ OPT_UP_U;
		else if (str[i] == 'j' && !(*opts & OPT_J))
			*opts = *opts ^ OPT_J;
		else
			return (1);
		i++;
	}
	return (0);
}

uint8_t	handle_sort(t_fhead *head, t_ulist e_l, t_ulist e_r, char *st)
{
	char	*f_str;
	char	*s_str;

	f_str = st + e_l.nl.n_un.n_strx;
	s_str = st + e_r.nl.n_un.n_strx;
	if (head->opts & OPT_N && !(head->opts & OPT_R))
		return (e_l.nl.n_value < e_r.nl.n_value);
	else if (head->opts & OPT_N)
		return (e_l.nl.n_value >= e_r.nl.n_value);
	else if (head->opts & OPT_R)
	{
		return (ft_strcmp(f_str, s_str) > 0 || (ft_strequ(f_str, s_str)
			&& e_l.nl.n_value > e_r.nl.n_value));
	}
	return (ft_strcmp(f_str, s_str) < 0
		|| (ft_strequ(f_str, s_str) && e_l.nl.n_value < e_r.nl.n_value));
}

int		invalid_parameters(int ac, char **av, uint16_t *opts, char *files[])
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

uint8_t	handle_file(char *arg, uint8_t res, uint16_t opts)
{
	int			fd;
	char		*ptr;
	struct stat	buf;
	uint8_t		error;

	error = 0;
	if ((fd = open(arg, O_RDONLY)) < 0)
		error = handle_error(arg, E_UNDIF, opts);
	else
	{
		if (fstat(fd, &buf) != 0)
			error = handle_error(arg, E_UNDIF_FILE, opts);
		else
		{
			if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
					== MAP_FAILED)
				error = handle_error(arg, E_UNDIF_FILE, opts);
			else
				error = handle_architecture(arg, ptr, buf.st_size, opts);
		}
		if (close(fd) != 0)
			error = handle_error(arg, E_UNDIF_FILE, opts);
	}
	return (res == 1 || error == 1 ? 1 : 0);
}
