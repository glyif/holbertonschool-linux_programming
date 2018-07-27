#include "elf_ph_utils.h"

/**
 *	loads an Elf program header from the file and saves it in the generic Elf
 *	program header structure hdr, the file is assumed to be in the given class
 */
void load_ph_header(Elf_Phdr *hdr, uint8_t class, uint8_t data, FILE *file)
{
	Elf32_Phdr *ptr32;
	Elf64_Phdr *ptr64;

	if (class == ELFCLASS32)
	{
		fread(hdr->data, sizeof(Elf32_Phdr), 1, file);   /* read the header */
		ptr32 = (Elf32_Phdr *)(hdr->data);
		hdr->p_type = ptr32->p_type;
		hdr->p_flags = ptr32->p_flags;

		hdr->p_offset = &(ptr32->p_offset);
		hdr->p_vaddr = &(ptr32->p_vaddr);
		hdr->p_paddr = &(ptr32->p_paddr);
		hdr->p_filesz = &(ptr32->p_filesz);
		hdr->p_memsz = &(ptr32->p_memsz);
		hdr->p_align = &(ptr32->p_align);
	}
	else /*if (class == ELFCLASS64)*/
	{
		fread(hdr->data, sizeof(Elf64_Phdr), 1, file);   /* read the header */
		ptr64 = (Elf64_Phdr *)(hdr->data);
		hdr->p_type = ptr64->p_type;
		hdr->p_flags = ptr64->p_flags;

		hdr->p_offset = &(ptr64->p_offset);
		hdr->p_vaddr = &(ptr64->p_vaddr);
		hdr->p_paddr = &(ptr64->p_paddr);
		hdr->p_filesz = &(ptr64->p_filesz);
		hdr->p_memsz = &(ptr64->p_memsz);
		hdr->p_align = &(ptr64->p_align);
	}
	if (data == ELFDATA2MSB)
		ph_header_to_little(class, hdr);  /* convert all fields to little endian*/
}

/* convert all fields in the program header structure to little endian */
void ph_header_to_little(uint8_t class, Elf_Phdr *pheader)
{
	/* convert all fields to little endian*/
	big_to_little((void *)&pheader->p_type, 32);
	big_to_little((void *)&pheader->p_flags, 32);
	if (class == ELFCLASS32)
	{
		big_to_little(pheader->p_offset, 32);
		big_to_little(pheader->p_vaddr, 32);
		big_to_little(pheader->p_paddr, 32);
		big_to_little(pheader->p_filesz, 32);
		big_to_little(pheader->p_memsz, 32);
		big_to_little(pheader->p_align, 32);
	}
	else
	{
		big_to_little(pheader->p_offset, 64);
		big_to_little(pheader->p_vaddr, 64);
		big_to_little(pheader->p_paddr, 64);
		big_to_little(pheader->p_filesz, 64);
		big_to_little(pheader->p_memsz, 64);
		big_to_little(pheader->p_align, 64);
	}
}

