# nm-otool

42 school project: reproduce the nm and otool commands.

## Setup
Launch the Makefile command in the root repository : `make`

When **ft_nm** and **ft_otool** binaries are created, you can use them like the original commands.

For example :
```
./ft_nm /usr/bin/bash /bin/git
./ft_otool -t /usr/bin/bash /bin/git
```

## What it does ?
The aim of the project is to understand the architecture of objects files like binaries, dynamic libraries, archives.
We must understand all parts of Mach-O files and universal binaries (with fat headers).

**ft_nm** displays the symbol table of an object file in this order : symbol value, symbol type and symbol name.

Options for **ft_nm**:

- -A : Display the object file name before all symbols

- -j : Display only the symbol name

- -g : Display external symbols

- -n : Sort symbols by values

- -p : No sort

- -r : Reverse sort

- -u : Display only undefined symbols

- -U : Don't display undefined symbols



**ft_otool** displays a specific part of an object file / librairy.

Options for **ft_otool**:

- -t : Display the content of __text section

- -d : Display the content of __data section

- -h : Display the Mach header

Holy Bible of Mach-O format : [OS X ABI Mach-O File Format Reference](https://pewpewthespells.com/re/Mach-O_File_Format.pdf)
