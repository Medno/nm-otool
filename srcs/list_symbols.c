#include "lib_nm_otool.h"

uint8_t	list_symbols(char *ptr)
{
	uint32_t	magic_number;

	magic_number = *(int *)ptr;
	if (magic_number == MH_MAGIC_64)
		ft_printf("Je suis magic 64b\n");
	return (0);
}
