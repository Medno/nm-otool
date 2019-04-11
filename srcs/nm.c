/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 17:49:47 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/11 18:30:38 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"


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

	opts = FT_NM;
	if ((nb_files = invalid_parameters(ac, av, &opts, files)) == -1)
		return (handle_error(files[0], E_UNDIF_OPT));
	ft_printf("Opts : %b\n", opts);
	return (nm(files, nb_files, opts));
}
