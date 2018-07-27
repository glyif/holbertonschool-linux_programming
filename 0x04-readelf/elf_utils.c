#include "elf_utils.h"

/**
 *	converts the number pointed by *num from big endian to little endian and
 *	updates it, uses the given size in bits
 */
void big_to_little(void *num, int size)
{
	uint8_t *p;
	uint8_t tmp;
	int i;
	int n;

	p = (uint8_t *)num;
	n = size / 8;  /* n is  the number of bytes */
	for (i = 0; i < n / 2; i++)
	{
		tmp = p[i];       /* swaps lowest bytes for the highest */
		p[i] = p[n - 1 - i];
		p[n - 1 - i] = tmp;
	}
}

/* returns the name for a given id on a translation table */
char *translate(int id, translation_table_t *ttable)
{
	int i;

	i = 0;

	while (ttable[i].id != -1 && ttable[i].id != id)
		i++;

	return (ttable[i].name);
}

/**
 *	loads an Elf header from the file and saves it in the generic Elf header
 *	structure hdr, the file is assumed to be in the given class
 */
void load_header(Elf_Ehdr *hdr, uint8_t class, FILE *file)
{
	Elf32_Ehdr *ptr32;
	Elf64_Ehdr *ptr64;

	if (class == ELFCLASS32)
	{
		fread(hdr->data, sizeof(Elf32_Ehdr), 1, file);   /* read the header */
		ptr32 = (Elf32_Ehdr *)(hdr->data);
		hdr->e_ident = ptr32->e_ident;
		hdr->e_type = ptr32->e_type;
		hdr->e_machine = ptr32->e_machine;
		hdr->e_version = ptr32->e_version;
		hdr->e_entry = &(ptr32->e_entry);
		hdr->e_phoff = &(ptr32->e_phoff);
		hdr->e_shoff = &(ptr32->e_shoff);
		hdr->e_flags = ptr32->e_flags;
		hdr->e_ehsize = ptr32->e_ehsize;
		hdr->e_phentsize = ptr32->e_phentsize;
		hdr->e_phnum = ptr32->e_phnum;
		hdr->e_shentsize = ptr32->e_shentsize;
		hdr->e_shnum = ptr32->e_shnum;
		hdr->e_shstrndx = ptr32->e_shstrndx;
	}
	else
	{
		fread(hdr->data, sizeof(Elf64_Ehdr), 1, file);   /* read the header */
		ptr64 = (Elf64_Ehdr *)(hdr->data);
		hdr->e_ident = ptr64->e_ident;
		hdr->e_type = ptr64->e_type;
		hdr->e_machine = ptr64->e_machine;
		hdr->e_version = ptr64->e_version;
		hdr->e_entry = &(ptr64->e_entry);
		hdr->e_phoff = &(ptr64->e_phoff);
		hdr->e_shoff = &(ptr64->e_shoff);
		hdr->e_flags = ptr64->e_flags;
		hdr->e_ehsize = ptr64->e_ehsize;
		hdr->e_phentsize = ptr64->e_phentsize;
		hdr->e_phnum = ptr64->e_phnum;
		hdr->e_shentsize = ptr64->e_shentsize;
		hdr->e_shnum = ptr64->e_shnum;
		hdr->e_shstrndx = ptr64->e_shstrndx;
	}
	if (hdr->e_ident[EI_DATA] == ELFDATA2MSB)
		/* convert all fields to little endian*/
		header_to_little(hdr);
}

/* convert all fields in the header structure to little endian */
void header_to_little(Elf_Ehdr *header)
{
	/* convert all fields to little endian*/
	big_to_little((void *)&header->e_type, 16);
	big_to_little((void *)&header->e_machine, 16);
	big_to_little((void *)&header->e_version, 32);
	if (header->e_ident[EI_CLASS] == ELFCLASS32)
	{
		big_to_little(header->e_entry, 32);
		big_to_little(header->e_phoff, 32);
		big_to_little(header->e_shoff, 32);
	}
	else
	{
		big_to_little(header->e_entry, 64);
		big_to_little(header->e_phoff, 64);
		big_to_little(header->e_shoff, 64);
	}
	big_to_little((void *)&header->e_flags, 32);
	big_to_little((void *)&header->e_ehsize, 16);
	big_to_little((void *)&header->e_phentsize, 16);
	big_to_little((void *)&header->e_phnum, 16);
	big_to_little((void *)&header->e_shentsize, 16);
	big_to_little((void *)&header->e_shnum, 16);
	big_to_little((void *)&header->e_shstrndx, 16);
}

