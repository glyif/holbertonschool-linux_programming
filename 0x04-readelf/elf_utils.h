#ifndef ELF_UTILS_H
#define ELF_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include "elf.h"

void big_to_little(void *num, int size);
char *translate(int id, translation_table_t *ttable);
void load_header(Elf_Ehdr *hdr, uint8_t class, FILE *file);
void header_to_little(Elf_Ehdr *header);

#endif /* ELF_UTILS_H */
