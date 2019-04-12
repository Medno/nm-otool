/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:47 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/12 15:01:50 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

int		nm(char *files[], int n_files, uint16_t opts)
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
	uint16_t	opts;
	char		*files[ac];
	int			nb_files;

	opts = FT_NM;
	if ((nb_files = invalid_parameters(ac, av, &opts, files)) == -1)
	{
		ft_dprintf(2,
			"ft_nm: Unknown command line argument '%s'. Try ft_nm -h for help\n"
			, files[0]);
		return (1);
	}
	return (nm(files, nb_files, opts));
}
