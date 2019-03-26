#include "lib_nm_otool.h"

uint8_t	handle_error(char *path)
{
	ft_dprintf(2, "nm: %s: Cannot open file or directory.\n", path);
	return (1);
}
