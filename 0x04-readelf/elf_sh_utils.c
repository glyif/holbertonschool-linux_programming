#include "elf_sh_utils.h"

void print_sh_flags(uint8_t class, uint64_t flags)
{
	char fstr[15];
	int pos;

	pos = 0;

	if (flags & SHF_WRITE)
	{
		fstr[pos++] = 'W';
		flags &= ~SHF_WRITE; /* clear flag */
	}

	if (flags & SHF_ALLOC)
	{
		fstr[pos++] = 'A';
		flags &= ~SHF_ALLOC; /* clear flag */
	}

	if (flags & SHF_EXECINSTR)
	{
		fstr[pos++] = 'X';
		flags &= ~SHF_EXECINSTR; /* clear flag */
	}

	if (flags & SHF_MERGE)
	{
		fstr[pos++] = 'M';
		flags &= ~SHF_MERGE; /* clear flag */
	}
	if (flags & SHF_STRINGS)
	{
		fstr[pos++] = 'S';
		flags &= ~SHF_STRINGS; /* clear flag */
	}
	if ((flags & SHF_X86_64_LARGE) && class == ELFCLASS64)
	{
		fstr[pos++] = 'l';
		flags &= ~SHF_X86_64_LARGE; /* clear flag */
	}
	if (flags & SHF_INFO_LINK)
	{
		fstr[pos++] = 'I';
		flags &= ~SHF_INFO_LINK; /* clear flag */
	}
	if (flags & SHF_LINK_ORDER)
	{
		fstr[pos++] = 'L';
		flags &= ~SHF_LINK_ORDER; /* clear flag */
	}
	if (flags & SHF_GROUP)
	{
		fstr[pos++] = 'G';
		flags &= ~SHF_GROUP; /* clear flag */
	}
	if (flags & SHF_TLS)
	{
		fstr[pos++] = 'T';
		flags &= ~SHF_TLS; /* clear flag */
	}
	if (flags & SHF_EXCLUDE)
	{
		fstr[pos++] = 'E';
		flags &= ~SHF_EXCLUDE; /* clear flag */
	}
	if (flags & SHF_OS_NONCONFORMING)
	{
		fstr[pos++] = 'O';
		flags &= ~SHF_OS_NONCONFORMING; /* clear flag */
	}
	if (flags & SHF_MASKOS)
	{
		fstr[pos++] = 'o';
		flags &= ~SHF_MASKOS; /* clear flag */
	}
	if (flags & SHF_MASKPROC)
	{
		fstr[pos++] = 'p';
		flags &= ~SHF_MASKPROC; /* clear flag */
	}
	if (flags)
		fstr[pos++] = 'x';

	fstr[pos] = 0;

	printf("%3s", fstr);
}

/* Obtains the section header name at position n in the string table */
void get_sh_name(uint16_t n, Elf_Ehdr *header, int class, FILE *file,
				char *name)
{
	uint64_t strpos;
	char c;
	int pos;
	Elf_Shdr sheader;

	if (class == ELFCLASS32)
		/* calculate position of strtable section*/
		strpos = I32(header->e_shoff) +
					header->e_shstrndx * header->e_shentsize;
	else
		/* calculate position of strtable section*/
		strpos = I64(header->e_shoff) +
					header->e_shstrndx * header->e_shentsize;
	/* position pointer at start of string table section header */
	fseek(file, strpos, SEEK_SET);
	/* read strtable section header */
	load_sh_header(&sheader, class, header->e_ident[EI_DATA], file);

	if (class == ELFCLASS32)
		/* position pointer at start of string */
		fseek(file, I32(sheader.sh_offset) + n, SEEK_SET);
	else
		/* position pointer at start of string */
		fseek(file, I64(sheader.sh_offset) + n, SEEK_SET);
	pos = 0;
	do {
		c = fgetc(file); /* get a string char */
		name[pos++] = c;
	} while (c != 0);   /* while not end of string*/
}

/**
 *	loads an Elf section header from the file and saves it in the generic Elf
 *	section header structure hdr, the file is assumed to be in the given class
 */
void load_sh_header(Elf_Shdr *hdr, uint8_t class, uint8_t data, FILE *file)
{
	Elf32_Shdr *ptr32;
	Elf64_Shdr *ptr64;

	if (class == ELFCLASS32)
	{
		fread(hdr->data, sizeof(Elf32_Shdr), 1, file);   /* read the header */
		ptr32 = (Elf32_Shdr *)(hdr->data);
		hdr->sh_name = ptr32->sh_name;
		hdr->sh_type = ptr32->sh_type;
		hdr->sh_flags = &(ptr32->sh_flags);
		hdr->sh_addr = &(ptr32->sh_addr);
		hdr->sh_offset = &(ptr32->sh_offset);
		hdr->sh_size = &(ptr32->sh_size);
		hdr->sh_link = ptr32->sh_link;
		hdr->sh_info = ptr32->sh_info;
		hdr->sh_addralign = &(ptr32->sh_addralign);
		hdr->sh_entsize = &(ptr32->sh_entsize);
	}
	else /*if (class == ELFCLASS64)*/
	{
		fread(hdr->data, sizeof(Elf64_Shdr), 1, file);   /* read the header */
		ptr64 = (Elf64_Shdr *)(hdr->data);
		hdr->sh_name = ptr64->sh_name;
		hdr->sh_type = ptr64->sh_type;
		hdr->sh_flags = &(ptr64->sh_flags);
		hdr->sh_addr = &(ptr64->sh_addr);
		hdr->sh_offset = &(ptr64->sh_offset);
		hdr->sh_size = &(ptr64->sh_size);
		hdr->sh_link = ptr64->sh_link;
		hdr->sh_info = ptr64->sh_info;
		hdr->sh_addralign = &(ptr64->sh_addralign);
		hdr->sh_entsize = &(ptr64->sh_entsize);
	}
	if (data == ELFDATA2MSB)
		/* convert all fields to little endian*/
		sh_header_to_little(class, hdr);
}

/* convert all fields in the section header structure to little endian */
void sh_header_to_little(uint8_t class, Elf_Shdr *sheader)
{
	/* convert all fields to little endian*/
	big_to_little((void *)&sheader->sh_name, 32);
	big_to_little((void *)&sheader->sh_type, 32);
	big_to_little((void *)&sheader->sh_link, 32);
	big_to_little((void *)&sheader->sh_info, 32);
	if (class == ELFCLASS32)
	{
		big_to_little(sheader->sh_flags, 32);
		big_to_little(sheader->sh_addr, 32);
		big_to_little(sheader->sh_offset, 32);
		big_to_little(sheader->sh_size, 32);
		big_to_little(sheader->sh_addralign, 32);
		big_to_little(sheader->sh_entsize, 32);
	}
	else
	{
		big_to_little(sheader->sh_flags, 64);
		big_to_little(sheader->sh_addr, 64);
		big_to_little(sheader->sh_offset, 64);
		big_to_little(sheader->sh_size, 64);
		big_to_little(sheader->sh_addralign, 64);
		big_to_little(sheader->sh_entsize, 64);
	}
}

