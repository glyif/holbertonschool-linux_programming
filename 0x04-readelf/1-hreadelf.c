#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
#include "elf_utils.h"
#include "elf_sh_utils.h"

/**
 * print_section_header - prints header info
 * @n: section in header
 * @header: header
 * @class: class to match
 * @file: file descriptor to read from
 */
void print_section_header(uint16_t n, Elf_Ehdr *header, int class, FILE *file)
{
	translation_table_t sh_type_ttable[] = {
		{SHT_NULL, "NULL"},
		{SHT_PROGBITS, "PROGBITS"},
		{SHT_SYMTAB, "SYMTAB"},
		{SHT_STRTAB, "STRTAB"},
		{SHT_RELA, "RELA"},
		{SHT_HASH, "HASH"},
		{SHT_DYNAMIC, "DYNAMIC"},
		{SHT_NOTE, "NOTE"},
		{SHT_NOBITS, "NOBITS"},
		{SHT_REL, "REL"},
		{SHT_SHLIB, "SHLIB"},
		{SHT_DYNSYM, "DYNSYM"},
		{SHT_INIT_ARRAY, "INIT_ARRAY"},
		{SHT_FINI_ARRAY, "FINI_ARRAY"},
		{SHT_PREINIT_ARRAY, "PREINIT_ARRAY"},
		{SHT_GROUP, "GROUP"},
		{SHT_SYMTAB_SHNDX, "SYMTAB_SHNDX"},
		{SHT_LOOS, "LOOS"},
		{SHT_GNU_ATTRIBUTES, "GNU_ATTRIBUTES"},
		{SHT_GNU_HASH, "GNU_HASH"},
		{SHT_GNU_verdef, "VERDEF"},
		{SHT_GNU_verneed, "VERNEED"},
		{SHT_GNU_versym, "VERSYM"},
		{SHT_LOPROC, "LOPROC"},
		{SHT_HIPROC, "HIPROC"},
		{SHT_LOUSER, "LOUSER"},
		{-1, "UNKNOWN"}
	};

	uint64_t shposition;
	Elf_Shdr sheader;

	if (class == ELFCLASS32)
		shposition = I32(header->e_shoff) + n * header->e_shentsize; /* calculate position of section n*/
	else
		shposition = I64(header->e_shoff) + n * header->e_shentsize; /* calculate position of section n*/

	fseek(file, shposition, SEEK_SET); /* position pointer at start of section header */
	load_sh_header(&sheader, header->e_ident[EI_CLASS], file); /* read section header */
	if (header->e_ident[EI_DATA] == ELFDATA2MSB)
		sh_header_to_little(class, &sheader);  /* convert all fields to little endian*/
	print_sh_name(sheader.sh_name, header, class, file);
	printf(" %-16s", translate(sheader.sh_type, sh_type_ttable));

	if (class == ELFCLASS32)
	{
		printf("%08x ", I32(sheader.sh_addr));
		printf("%06x ", I32(sheader.sh_offset));
		printf("%06x ", I32(sheader.sh_size));
		printf("%02x ", I32(sheader.sh_entsize));
		print_sh_flags(class, I32(sheader.sh_flags));
	}
	else
	{
		printf("%016lx ", I64(sheader.sh_addr));
		printf("%06lx ", I64(sheader.sh_offset));
		printf("%06lx ", I64(sheader.sh_size));
		printf("%02lx ", I64(sheader.sh_entsize));
		print_sh_flags(class, I64(sheader.sh_flags));
	}
	printf("%3d", sheader.sh_link);
	printf("%4d", sheader.sh_info);

	if (class == ELFCLASS32)
		printf("%3d", I32(sheader.sh_addralign));
	else
		printf("%3lu", I64(sheader.sh_addralign));
}

/**
 * print_section_info - prints elf section
 * @class: class to match with
 * @file: file descriptor to read from
 */
void print_section_info(uint8_t class, FILE *file)
{
	Elf_Ehdr header;
	int i;

	/* read the header */
	load_header(&header, class, file);
	if (header.e_ident[EI_DATA] == ELFDATA2MSB)
	{
		/* convert all fields to little endian*/
		header_to_little(&header);
	}
	printf("There are %d section headers, starting at offset ", header.e_shnum);

	if (class == ELFCLASS32)
		printf("0x%x:\n\n", I32(header.e_shoff));
	else
		printf("0x%lx:\n\n", I64(header.e_shoff));

	printf("Section Headers:\n");
	printf("  [Nr] Name              Type            ");
	if (class == ELFCLASS32)
		printf("Addr     Off    Size   ES Flg Lk Inf Al\n");
	else
		printf("Address          Off    Size   ES Flg Lk Inf Al\n");

	for (i = 0; i < header.e_shnum; i++)
	{
		/* read the current section header info */
		printf("  [%2d]", i);
		print_section_header(i, &header, class, file);
		printf("\n");
	}
	printf("Key to Flags:\n");
	printf("  W (write), A (alloc), X (execute), M (merge), S (strings)");

	if (class == ELFCLASS64 && header.e_machine == EM_X86_64)
		printf(", l (large)");

	printf("\n");
	printf("  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
	printf("  O (extra OS processing required) o (OS specific), p (processor specific)\n");
}

/**
 * main - main function
 * @argc: argument count
 * @argv: arguments array
 * Return: integer for fail / success
 */
int main(int argc, char **argv)
{
	FILE *file;
	char ident[ELF_NIDENT];

	if (argc != 2)
	{
		fprintf(stderr, "1-hreadelf: Error: Invalid number of arguments.\n");
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "    %s <elf-filename>\n", argv[0]);
		exit(1);
	}
	/* open the given filename */
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		fprintf(stderr, "1-hreadelf: Error: '%s': No such file\n", argv[1]);
		exit(1);
	}
	/* read the ident bytes at the start of the file*/
	fread(ident, ELF_NIDENT, 1, file);

	/* restore file pointer to start of file */
	fseek(file, 0, SEEK_SET);
	/* compare the 4 elf magic numbers */
	if (ident[EI_MAG0] != ELFMAG0 || ident[EI_MAG1] != ELFMAG1 ||
		ident[EI_MAG2] != ELFMAG2 || ident[EI_MAG3] != ELFMAG3)
	{
		fprintf(stderr, "1-hreadelf: Error: Not an ELF file - it has the wrong"
					   " magic bytes at the start\n");
		fclose(file);
		exit(1);
	}
	/* unknown class elf */
	if (ident[EI_CLASS] != ELFCLASS32 && ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "1-hreadelf: Error: Not a valid ELF file\n");
		fclose(file);
		exit(1);
	}

	print_section_info(ident[EI_CLASS], file);

	fclose(file);
	return (0);
}

