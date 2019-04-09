/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:47 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/09 15:28:43 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

int		handle_opts(char *str, uint8_t *opts, size_t size, char *files[])
{
	size_t	i;

	i = 1;
	while (i < size)
	{
		if (str[i] == 'a' && !(*opts & OPT_A))
			*opts = *opts ^ OPT_A;
		else if (str[i] == 'g' && !(*opts & OPT_G))
			*opts = *opts ^ OPT_G;
		else if (str[i] == 'n' && !(*opts & OPT_N))
			*opts = *opts ^ OPT_N;
		else if (str[i] == 'p' && !(*opts & OPT_P))
			*opts = *opts ^ OPT_P;
		else if (str[i] == 'r' && !(*opts & OPT_R))
			*opts = *opts ^ OPT_R;
		else if (str[i] == 'u' && !(*opts & OPT_U))
			*opts = *opts ^ OPT_U;
		else
		{
			files[0] = str;
			return (1);
		}
		i++;
	}
	return (0);
}

int		invalid_parameters(int ac, char **av, uint8_t *opts, char *files[])
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
			else if (handle_opts(av[i], opts, ft_strlen(av[i]), files))
				return (-1);
		}
		else
		{
			files[nb_files] = av[i];
			nb_files++;
		}
		i++;
	}
	return (nb_files);
}

uint8_t	handle_file(char *arg, uint8_t res, uint8_t opts)
{
	int			fd;
	char		*ptr;
	struct stat	buf;
	uint8_t		error;

	error = 0;
	if ((fd = open(arg, O_RDONLY)) < 0)
		error = handle_error(arg, E_UNDIF_FILE);
	else
	{
		if (fstat(fd, &buf) != 0)
			error = handle_error(arg, E_UNDIF_FILE);
		else
		{
			if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0))
					== MAP_FAILED)
				error = handle_error(arg, E_UNDIF_FILE);
			else
				error = handle_architecture(arg, ptr, buf.st_size, opts);
		}
		if (close(fd) != 0)
			error = handle_error(arg, E_UNDIF_FILE);
	}
	return (res == 1 || error == 1 ? 1 : 0);
}

int		nm(char *files[], int n_files, uint8_t opts)
{
	uint8_t	ret;
	int		i;

	i = 0;
	ret = 0;
	if (!n_files)
		ret = handle_file("a.out", ret, opts);
	while (i < n_files)
	{
		ret = handle_file(files[i], ret, opts);
		i++;
	}
	return (ret);
}

int		main(int ac, char **av)
{
	uint8_t	opts;
	char	*files[ac];
	int		nb_files;

	opts = 0;
	if ((nb_files = invalid_parameters(ac, av, &opts, files)) == -1)
		return (handle_error(files[0], E_UNDIF_OPT));
	ft_printf("Opts : %b\n", opts);
	return (nm(files, nb_files, opts));
}
