/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbols_struct.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pchadeni <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/03 16:21:54 by pchadeni          #+#    #+#             */
/*   Updated: 2019/04/11 17:18:35 by pchadeni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lib_nm_otool.h"

int	otool(char *files[], int n_files, uint8_t opts)
{
	uint8_t	ret;
	int		i;

	i = 0;
	ret = 0;
	while (i < n_files)
	{
		ret = handle_file(files[i], ret, opts);
		i++;
	}
	return (ret);
}

int	main(int ac, char **av)
{
	uint8_t	opts;
	char	*files[ac];
	int		nb_files;

	opts = FT_OTOOL;
	if ((nb_files = invalid_parameters(ac, av, &opts, files)) == -1)
		return (handle_error(files[0], E_UNDIF_OPT));
	return (otool(files, nb_files, opts));
}
