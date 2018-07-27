#ifndef ELF_PH_UTILS_H
#define ELF_PH_UTILS_H

#include <string.h>
#include "elf.h"
#include "elf_utils.h"

void ph_header_to_little(uint8_t class, Elf_Phdr *sheader);
void load_ph_header(Elf_Phdr *hdr, uint8_t class, uint8_t data, FILE *file);

#endif /* ELF_PH_UTILS_H */

