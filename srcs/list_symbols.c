#include "lib_nm_otool.h"

void	display_sym_tab(char *ptr, struct load_command *lc)
{
	uint32_t				i;
	char					*sym_tab;
	struct symtab_command	*sc;
	struct nlist_64			*nlist;

	sc = (struct symtab_command *)lc;
	nlist = (void *)ptr + sc->symoff;
	sym_tab = (void *)ptr + sc->stroff;

	i = 0;
	while (i < sc->nsyms)
	{
		ft_printf("%s\n", sym_tab + nlist[i].n_un.n_strx);
		i++;
	}
}

uint8_t	list_symbols(char *ptr)
{
	struct mach_header_64	*macho_64;
	struct load_command		*lc;
	uint32_t				i;

	i = 0;
	macho_64 = (struct mach_header_64 *)ptr;
	if (macho_64->magic == MH_MAGIC_64)
		ft_printf("Je suis magic 64b\n");
	lc = (void *)ptr + sizeof(*macho_64);
	while (i < macho_64->ncmds)
	{
		if (lc->cmd == LC_SYMTAB)
			display_sym_tab(ptr, lc);
		lc = (void *)lc + lc->cmdsize;
		i++;
	}
	
	return (0);
}
