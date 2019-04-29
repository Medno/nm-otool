/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:21:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/29 14:58:36 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

int	otool(char *files[], int n_files, uint16_t opts)
{
	uint8_t	ret;
	int		i;

	i = 0;
	ret = 0;
	if (!n_files)
	{
		ft_printf("ft_otool: at least one file must be specified\n");
		ret = EXIT_FAILURE;
	}
	while (i < n_files)
	{
		ret = handle_file(files[i], ret, opts);
		i++;
	}
	return (ret);
}

int	main(int ac, char **av)
{
	uint16_t	opts;
	char		*files[ac];
	int			nb_files;

	opts = FT_OTOOL;
	if ((nb_files = invalid_parameters(ac, av, &opts, files)) == -1)
	{
		ft_dprintf(2,
				"ft_nm: Unknown command line argument '%s'.\
				Try ft_nm -h for help\n",
				files[0]);
		return (1);
	}
	return (otool(files, nb_files, opts));
}
