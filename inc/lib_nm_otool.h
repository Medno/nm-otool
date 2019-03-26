#ifndef LIB_NM_OTOOL_H
# define LIB_NM_OTOOL_H

# include "libft.h"
# include <fcntl.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>

uint8_t		list_symbols(char *ptr);

uint8_t		handle_error(char *path);
#endif
