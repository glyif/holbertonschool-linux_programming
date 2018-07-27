#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
#include "elf_utils.h"
#include "elf_sh_utils.h"
#include "elf_ph_utils.h"

/**
 * print_program_header - prints program header info
 * @n: number of program section in program header table
 * @header: header
 * @class: class assumed for the file (32 or 64 bits)
 * @file: file descriptor to read from
 */
void print_program_header(uint16_t n, Elf_Ehdr *header, int class, FILE *file)
{
	uint64_t phposition;
	Elf_Phdr pheader;
	char name[100];
	int pos;
	char *p;

	translation_table_t ph_type_ttable[] = {
		{PT_NULL, "NULL"},
		{PT_LOAD, "LOAD"},
		{PT_DYNAMIC, "DYNAMIC"},
		{PT_INTERP, "INTERP"},
		{PT_NOTE, "NOTE"},
		{PT_SHLIB, "SHLIB"},
		{PT_PHDR, "PHDR"},
		{PT_TLS, "TLS"},
		{PT_NUM, "NUM"},
		{PT_LOOS, "LOOS"},
		{PT_GNU_EH_FRAME, "GNU_EH_FRAME"},
		{PT_GNU_STACK, "GNU_STACK"},
		{PT_GNU_RELRO, "GNU_RELRO"},
		{PT_LOSUNW, "LOSUNW"},
		{PT_SUNWBSS, "SUNWBSS"},
		{PT_SUNWSTACK, "SUNWSTACK"},
		{PT_HIOS, "HIOS"},
		{PT_LOPROC, "LOPROC"},
		{PT_HIPROC, "HIPROC"},
		{-1, ""}
	};

	if (class == ELFCLASS32)
		/* calculate position of section n*/
		phposition = I32(header->e_phoff) + n * header->e_phentsize;
	else
		/* calculate position of section n*/
		phposition = I64(header->e_phoff) + n * header->e_phentsize;

	/* position pointer at start of section header */
	fseek(file, phposition, SEEK_SET);
	load_ph_header(&pheader, header->e_ident[EI_CLASS],
		header->e_ident[EI_DATA], file); /* read section header */
	p = translate(pheader.p_type, ph_type_ttable);
	if (p[0])
		printf("  %-15s", p);
	else
	{
		sprintf(name, "LOOS+%x", pheader.p_type - PT_LOOS);
		printf("  %-15s", name);
	}
	if (class == ELFCLASS32)
	{
		printf("0x%06x ", I32(pheader.p_offset));
		printf("0x%08x ", I32(pheader.p_vaddr));
		printf("0x%08x ", I32(pheader.p_paddr));
		printf("0x%05x ", I32(pheader.p_filesz));
		printf("0x%05x ", I32(pheader.p_memsz));
	}
	else
	{
		printf("0x%06lx ", I64(pheader.p_offset));
		printf("0x%016lx ", I64(pheader.p_vaddr));
		printf("0x%016lx ", I64(pheader.p_paddr));
		printf("0x%06lx ", I64(pheader.p_filesz));
		printf("0x%06lx ", I64(pheader.p_memsz));
	}
	printf("%c%c%c ", (pheader.p_flags & PF_R) ? 'R' : ' ',
		   (pheader.p_flags & PF_W) ? 'W' : ' ',
		   (pheader.p_flags & PF_X) ? 'E' : ' ');

	if (class == ELFCLASS32)
		printf("0x%x", I32(pheader.p_align));
	else
		printf("0x%lx", I64(pheader.p_align));
	if (pheader.p_type == PT_INTERP)
	{
		if (class == ELFCLASS32)
			/* position pointer at start of string */
			fseek(file, I32(pheader.p_offset), SEEK_SET);
		else
			/* position pointer at start of string */
			fseek(file, I64(pheader.p_offset), SEEK_SET);
		pos = 0;
		while ((name[pos++] = fgetc(file)) != 0)
			; /* get string chars */
		printf("\n      [Requesting program interpreter: %s]", name);
	}
}

void print_section_segments(uint16_t n, Elf_Ehdr *header, int class,
							FILE *file)
{
	uint64_t phposition;
	Elf_Phdr pheader;
	uint64_t start;
	uint64_t end;
	uint64_t shposition;
	Elf_Shdr sheader;
	uint64_t soffset;
	int i;
	char name[MAX_NAME_LEN];

	if (class == ELFCLASS32)
		/* calculate position of section n*/
		phposition = I32(header->e_phoff) + n * header->e_phentsize;
	else
		/* calculate position of section n*/
		phposition = I64(header->e_phoff) + n * header->e_phentsize;

	/* position pointer at start of section header */
	fseek(file, phposition, SEEK_SET);
	load_ph_header(&pheader, header->e_ident[EI_CLASS],
		header->e_ident[EI_DATA], file); /* read section header */
	start = (class == ELFCLASS32) ?
				I32(pheader.p_offset) : I64(pheader.p_offset);
	end = start;
	end += (class == ELFCLASS32) ? I32(pheader.p_filesz) :
									I64(pheader.p_filesz);

	shposition = (class == ELFCLASS32) ? I32(header->e_shoff) :
											I64(header->e_shoff);

	for (i = 1; i < header->e_shnum; i++)
	{
		/* position pointer at start of section header */
		fseek(file, shposition + i * header->e_shentsize, SEEK_SET);
		/* read section header */
		load_sh_header(&sheader, class, header->e_ident[EI_DATA], file);
		soffset = (class == ELFCLASS32) ?
						I32(sheader.sh_offset) : I64(sheader.sh_offset);
		if ((soffset >= start && soffset < end) ||
			(soffset == end && sheader.sh_type == SHT_NOBITS))
		{
			get_sh_name(sheader.sh_name, header, class, file, name);
			printf(" %s", name);
		}
	}

}

/**
 * print_pheader_info - prints elf program header
 * @class: class assumed for the file (32 or 64 bits)
 * @file: file descriptor to read from
 */
void print_pheader_info(uint8_t class, FILE *file)
{
	Elf_Ehdr header;
	int i;
	translation_table_t type_ttable[] = {
		{ET_NONE, "NONE (No file type)"},
		{ET_REL, "REL (Relocatable file)"},
		{ET_EXEC, "EXEC (Executable file)"},
		{ET_DYN, "DYN (Shared object file)"},
		{ET_CORE, "CORE (Core file)"},
		{ET_LOOS, "LOOS (Operating system-specific)"},
		{ET_HIOS, "HIOS (Operating system-specific)"},
		{ET_LOPROC, "LOPROC (Processor-specific)"},
		{ET_HIPROC, "HIPROC (Processor-specific)"},
		{-1, ""}
	};

	/* read the header */
	load_header(&header, class, file);

	printf("ELF file type is %s\n", translate(header.e_type, type_ttable));
	if (class == ELFCLASS32)
		printf("Entry point 0x%x\n", I32(header.e_entry));
	else
		printf("Entry point 0x%lx\n", I64(header.e_entry));
	printf("There are %d program headers, starting at offset ", header.e_phnum);

	if (class == ELFCLASS32)
		printf("%u\n\n", I32(header.e_phoff));
	else
		printf("%lu\n\n", I64(header.e_phoff));

	printf("Program Headers:\n");
	printf("  Type           ");
	if (class == ELFCLASS32)
		printf("Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align\n");
	else
	{
		printf("Offset   VirtAddr           PhysAddr           FileSiz");
		printf("  MemSiz   Flg Align\n");
	}
	for (i = 0; i < header.e_phnum; i++)
	{
		/* read the current program header info */
		print_program_header(i, &header, class, file);
		printf("\n");
	}
	printf("\n Section to Segment mapping:\n");
	printf("  Segment Sections...\n");
	printf("   00\n");
	for (i = 1; i < header.e_phnum; i++)
	{
		/* read the current section header info */
		printf("   %02d    ", i);
		print_section_segments(i, &header, class, file);
		printf("\n");
	}
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
		fprintf(stderr, "2-hreadelf: Error: Invalid number of arguments.\n");
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "    %s <elf-filename>\n", argv[0]);
		exit(1);
	}
	/* open the given filename */
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		fprintf(stderr, "2-hreadelf: Error: '%s': No such file\n", argv[1]);
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
		fprintf(stderr, "2-hreadelf: Error: Not an ELF file");
		fprintf(stderr, " - it has the wrong magic bytes at the start\n");
		fclose(file);
		exit(1);
	}
	/* unknown class elf */
	if (ident[EI_CLASS] != ELFCLASS32 && ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "2-hreadelf: Error: Not a valid ELF file\n");
		fclose(file);
		exit(1);
	}

	print_pheader_info(ident[EI_CLASS], file);

	fclose(file);
	return (0);
}

