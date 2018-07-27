#ifndef ELF_SH_UTILS_H
#define ELF_SH_UTILS_H

#include <string.h>
#include "elf.h"
#include "elf_utils.h"

void print_sh_flags(uint8_t class, uint64_t flags);
void sh_header_to_little(uint8_t class, Elf_Shdr *sheader);
void load_sh_header(Elf_Shdr *hdr, uint8_t class, uint8_t data, FILE *file);
void get_sh_name(uint16_t n, Elf_Ehdr *header, int class, FILE *file,
				char *name);

#endif
