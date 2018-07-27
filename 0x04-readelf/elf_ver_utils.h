#ifndef ELF_VER_UTILS_H
#define ELF_VER_UTILS_H

#include <string.h>
#include "elf.h"
#include "elf_utils.h"

void load_verneed(Elf_Verneed *hdr, uint8_t data, FILE *file);
void load_vernaux(Elf_Vernaux *hdr, uint8_t data, FILE *file);

#endif /* ELF_VER_UTILS_H */

