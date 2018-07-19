all:	0-hreadelf 1-hreadelf

0-hreadelf:	0-hreadelf.c elf_utils.c
		gcc -Wall -Werror -Wextra -pedantic -o 0-hreadelf 0-hreadelf.c elf_utils.c

1-hreadelf:	1-hreadelf.c elf_utils.c elf_sh_utils.c
		gcc -Wall -Werror -Wextra -pedantic -o 1-hreadelf 1-hreadelf.c elf_utils.c elf_sh_utils.c

clean:
		rm -f *.o 0-hreadelf 1-hreadelf
