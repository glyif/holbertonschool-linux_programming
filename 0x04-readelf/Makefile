all:	2-hreadelf 100-hreadelf

2-hreadelf:	2-hreadelf.c elf_utils.c elf_ph_utils.c elf_sh_utils.c
		gcc -Wall -Werror -Wextra -pedantic -o 2-hreadelf 2-hreadelf.c elf_utils.c elf_ph_utils.c elf_sh_utils.c

100-hreadelf:	100-hreadelf.c elf_utils.c elf_sh_utils.c elf_sym_utils.c elf_ver_utils.c
		gcc -Wall -Werror -Wextra -pedantic -o 100-hreadelf 100-hreadelf.c elf_utils.c elf_sh_utils.c elf_sym_utils.c elf_ver_utils.c

clean:
		rm -f *.o 2-hreadelf 100-hreadelf

