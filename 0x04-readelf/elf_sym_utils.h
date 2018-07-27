#ifndef ELF_SYM_UTILS_H
#define ELF_SYM_UTILS_H

#include <string.h>
#include "elf.h"
#include "elf_utils.h"
#include "elf_sh_utils.h"

void sym_header_to_little(uint8_t class, Elf_Sym *sym);
void load_sym_entry(Elf_Sym *sym, uint8_t class, uint8_t data, FILE *file);
void get_sym_name(uint32_t n, Elf_Shdr *sheader, Elf_Ehdr *header, FILE *file,
				char *name);

#endif /* ELF_SYM_UTILS_H */

