#include "elf_sym_utils.h"

/**
 *	loads an Elf symbol entry from the file and saves it in the generic Elf
 *	symbol entry structure hdr, the file is assumed to be in the given class
 */
void load_sym_entry(Elf_Sym *sym, uint8_t class, uint8_t data, FILE *file)
{
	Elf32_Sym *ptr32;
	Elf64_Sym *ptr64;

	if (class == ELFCLASS32)
	{
		/* read the symbol entry*/
		fread(sym->data, sizeof(Elf32_Sym), 1, file);
		ptr32 = (Elf32_Sym *)(sym->data);
		sym->st_name = ptr32->st_name;
		sym->st_value = &(ptr32->st_value);
		sym->st_size = &(ptr32->st_size);
		sym->st_info = ptr32->st_info;
		sym->st_other = ptr32->st_other;
		sym->st_shndx = ptr32->st_shndx;
	}
	else /*if (class == ELFCLASS64)*/
	{
		/* read the symbol entry */
		fread(sym->data, sizeof(Elf64_Sym), 1, file);
		ptr64 = (Elf64_Sym *)(sym->data);
		sym->st_name = ptr64->st_name;
		sym->st_value = &(ptr64->st_value);
		sym->st_size = &(ptr64->st_size);
		sym->st_info = ptr64->st_info;
		sym->st_other = ptr64->st_other;
		sym->st_shndx = ptr64->st_shndx;
	}
	if (data == ELFDATA2MSB)
		/* convert all fields to little endian*/
		sym_header_to_little(class, sym);
}

/* convert all fields in the section header structure to little endian */
void sym_header_to_little(uint8_t class, Elf_Sym *sym)
{
	/* convert all fields to little endian*/
	big_to_little((void *)&sym->st_name, 32);
	big_to_little((void *)&sym->st_shndx, 16);
	if (class == ELFCLASS32)
	{
		big_to_little(sym->st_value, 32);
		big_to_little(sym->st_size, 32);
	}
	else
	{
		big_to_little(sym->st_value, 64);
		big_to_little(sym->st_size, 64);
	}
}

/* Obtains the string at position n in the given string table */
void get_sym_name(uint32_t n, Elf_Shdr *sheader, Elf_Ehdr *header, FILE *file,
				char *name)
{
	uint64_t strpos;
	char c;
	int pos;
	Elf_Shdr strheader;

	if (header->e_ident[EI_CLASS] == ELFCLASS32)
		/* calculate position of strtable section*/
		strpos = I32(header->e_shoff) + sheader->sh_link * header->e_shentsize;
	else
		/* calculate position of strtable section*/
		strpos = I64(header->e_shoff) + sheader->sh_link * header->e_shentsize;
	/* position pointer at start of string table section header */
	fseek(file, strpos, SEEK_SET);
	load_sh_header(&strheader, header->e_ident[EI_CLASS],
			header->e_ident[EI_DATA], file); /* read strtable section header */

	if (header->e_ident[EI_CLASS] == ELFCLASS32)
		/* position pointer at start of string */
		fseek(file, I32(strheader.sh_offset) + n, SEEK_SET);
	else
		/* position pointer at start of string */
		fseek(file, I64(strheader.sh_offset) + n, SEEK_SET);
	pos = 0;
	do {
		c = fgetc(file); /* get a string char */
		name[pos++] = c;
	} while (c != 0);   /* while not end of string*/
}

