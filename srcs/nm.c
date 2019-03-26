#include "lib_nm_otool.h"

int	invalid_parameters(int ac, char **av)
{
	int	i;

	i = 0;
	while (i < ac)
	{
		(void)av;
		i++;
	}
	return (0);
}

uint8_t	handle_file(char *arg, uint8_t res)
{
	int			fd;
	char		*ptr;
	struct stat	buf;
	uint8_t		error;

	error = 0;
	if ((fd = open(arg, O_RDONLY)) < 0)
		error = handle_error(arg);
	else 
	{
		ft_printf("fd opened: |%d|\n", fd);
		if (fstat(fd, &buf) != 0)
			error = handle_error(arg);
		else
		{
			ft_printf("Size of file: |%zu|\n", buf.st_size);
			if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
				error = handle_error(arg);
			else
				list_symbols(ptr);
		}
		if (close(fd) != 0)
			error = handle_error(arg);
	}
	return (res == 1 || error == 1 ? 1 : 0);
}

int	nm(int ac, char **av)
{
	uint8_t	ret;
	int		i;

	i = 1;
	ret = 0;
	while (i < ac)
	{
		if (av[i][0] != '-')
			ret = handle_file(av[i], ret);
		i++;
	}
	return (ret);
}

int	main(int ac, char **av)
{
	if (invalid_parameters(ac, av))
		return (EXIT_FAILURE);
	return (nm(ac, av));
}