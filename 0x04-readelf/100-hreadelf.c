#include <stdio.h>
#include <stdlib.h>
#include "elf.h"
#include "elf_utils.h"
#include "elf_sh_utils.h"
#include "elf_sym_utils.h"
#include "elf_ver_utils.h"

/**
 * print_symbol_version - prints the symbol version needed for a given symbol
 *							in a dynamic symbol table
 * @n: number of symbol entry in symbol table
 * @header: Elf header
 * @file: file descriptor to read from
 */
void print_symbol_version(uint32_t n, Elf_Ehdr *header, FILE *file)
{
	Elf_Shdr sheader;
	int i;
	uint64_t position;
	uint16_t ver_id;
	int found;
	Elf_Shdr  versym_hdr;
	Elf_Shdr  verneed_hdr;
	Elf_Verneed verneed;
	Elf_Vernaux vernaux;
	uint64_t curpos;
	uint64_t curpos2;
	char name[MAX_NAME_LEN];

	position = (header->e_ident[EI_CLASS] == ELFCLASS32) ?
					I32(header->e_shoff) : I64(header->e_shoff);
	found = 0;
	for (i = 0; i < header->e_shnum; i++)
	{
		/* position pointer at start of section header */
		fseek(file, position + i * header->e_shentsize, SEEK_SET);
		/* read section header */
		load_sh_header(&sheader, header->e_ident[EI_CLASS],
				header->e_ident[EI_DATA], file);
		if (sheader.sh_type == SHT_GNU_versym)
		{
			fseek(file, position + i * header->e_shentsize, SEEK_SET);
			load_sh_header(&versym_hdr, header->e_ident[EI_CLASS],
					header->e_ident[EI_DATA], file);
			found++;
		}
		if (sheader.sh_type == SHT_GNU_verneed)
		{
			fseek(file, position + i * header->e_shentsize, SEEK_SET);
			load_sh_header(&verneed_hdr, header->e_ident[EI_CLASS],
					header->e_ident[EI_DATA], file);
			found++;
		}
	}
	if (found == 2)
	{
		/* position pointer at start of section header */
		position = (header->e_ident[EI_CLASS] == ELFCLASS32) ?
				I32(versym_hdr.sh_offset) + n * I32(versym_hdr.sh_entsize) :
				I64(versym_hdr.sh_offset) + n * I64(versym_hdr.sh_entsize);
		fseek(file, position, SEEK_SET);
		fread(&ver_id, sizeof(uint16_t), 1, file); /* read id */
		position = (header->e_ident[EI_CLASS] == ELFCLASS32) ?
				I32(verneed_hdr.sh_offset) : I64(verneed_hdr.sh_offset);
		if (header->e_ident[EI_DATA] == ELFDATA2MSB)
			big_to_little((void *)&ver_id, 16);
		fseek(file, position, SEEK_SET);
		do {
			curpos = ftell(file);
			load_verneed(&verneed, header->e_ident[EI_DATA], file); /* read verneed */
			fseek(file, curpos + verneed.vn_aux, SEEK_SET);
			do {

				curpos2 = ftell(file);
				load_vernaux(&vernaux, header->e_ident[EI_DATA], file); /* read verneed */
				if (vernaux.vna_other == ver_id)
				{
					get_sym_name(vernaux.vna_name, &verneed_hdr, header, file, name);
					printf("@%s (%d)", name, ver_id);
					verneed.vn_next = 0;
					vernaux.vna_next = 0;
				}
				fseek(file, curpos2 + vernaux.vna_next, SEEK_SET);
			} while (vernaux.vna_next);
			fseek(file, curpos + verneed.vn_next, SEEK_SET);
		} while (verneed.vn_next);
	}
}

/**
 * print_symbol_entry - prints an entry from the elf symbol table
 * @n: number of section header in section header table
 * @sheader: Elf section header
 * @header: Elf header
 * @class: class assumed for the file (32 or 64 bits)
 * @file: file descriptor to read from
 */
void print_symbol_entry(uint32_t n, Elf_Shdr *sheader, Elf_Ehdr *header,
						int class, FILE *file)
{
	uint64_t sympos;
	Elf_Sym  symbol;
	char name[MAX_NAME_LEN];
	char *p;

	translation_table_t sym_type_ttable[] = {
		{STT_NOTYPE, "NOTYPE"},
		{STT_OBJECT, "OBJECT"},
		{STT_FUNC, "FUNC"},
		{STT_SECTION, "SECTION"},
		{STT_FILE, "FILE"},
		{STT_COMMON, "COMMON"},
		{STT_LOOS, "LOOS"},
		{STT_HIOS, "HIOS"},
		{STT_LOPROC, "LOPROC"},
		{STT_HIPROC, "HIPROC"},
		{-1, ""}
	};
	translation_table_t sym_bind_ttable[] = {
		{STB_LOCAL, "LOCAL"},
		{STB_GLOBAL, "GLOBAL"},
		{STB_WEAK, "WEAK"},
		{STB_LOOS, "LOOS"},
		{STB_HIOS, "HIOS"},
		{STB_LOPROC, "LOPROC"},
		{STB_HIPROC, "HIPROC"},
		{-1, ""}
	};
	translation_table_t sym_vis_ttable[] = {
		{STV_DEFAULT, "DEFAULT"},
		{STV_INTERNAL, "INTERNAL"},
		{STV_HIDDEN, "HIDDEN"},
		{STV_PROTECTED, "PROTECTED"},
		{-1, ""}
	};
	translation_table_t sym_shndx_ttable[] = {
		{SHN_UNDEF, "UND"},
		{SHN_LORESERVE, "LORESERVE"},
		{SHN_LOPROC, "LOPROC"},
		{SHN_HIPROC, "HIPROC"},
		{SHN_LOOS, "LOOS"},
		{SHN_HIOS, "HIOS"},
		{SHN_ABS, "ABS"},
		{SHN_COMMON, "COMMON"},
		{SHN_XINDEX, "XINDEX"},
		{SHN_HIRESERVE, "HIRESERVE"},
		{-1, ""}
	};
	sympos = (class == ELFCLASS32) ?
				I32(sheader->sh_offset) + n * I32(sheader->sh_entsize) :
				I64(sheader->sh_offset) + n * I64(sheader->sh_entsize);
	/* position pointer at start of section header */
	fseek(file, sympos, SEEK_SET);
	/* read symbol entry */
	load_sym_entry(&symbol, class, header->e_ident[EI_DATA], file);
	if (class == ELFCLASS32)
		printf(" %08x %5u", I32(symbol.st_value), I32(symbol.st_size));
	else
		printf(" %016lx %5lu", I64(symbol.st_value), I64(symbol.st_size));
	printf(" %-7s", translate(ST_TYPE(symbol.st_info), sym_type_ttable));
	printf(" %-6s", translate(ST_BIND(symbol.st_info), sym_bind_ttable));
	printf(" %-8s", translate(ST_VISIBILITY(symbol.st_other), sym_vis_ttable));
	p = translate(symbol.st_shndx, sym_shndx_ttable);
	if (!p[0])
		printf(" %3d", symbol.st_shndx);
	else
		printf(" %3s", p);
	get_sym_name(symbol.st_name, sheader, header, file, name);
	printf(" %s", name);
	if (sheader->sh_type == SHT_DYNSYM)
		print_symbol_version(n, header, file);
}

/**
 * print_symtable_info - prints elf symbol table
 * @class: class assumed for the file (32 or 64 bits)
 * @file: file descriptor to read from
 */
void print_symtable_info(uint8_t class, FILE *file)
{
	Elf_Ehdr header;
	int i;
	uint64_t shposition;
	Elf_Shdr sheader;
	uint64_t size;
	uint64_t j;
	char name[MAX_NAME_LEN];

	/* read the header */
	load_header(&header, class, file);

	shposition = (class == ELFCLASS32) ? I32(header.e_shoff) :
											I64(header.e_shoff);

	for (i = 0; i < header.e_shnum; i++)
	{
		/* position pointer at start of section header */
		fseek(file, shposition + i * header.e_shentsize, SEEK_SET);
		/* read section header */
		load_sh_header(&sheader, class, header.e_ident[EI_DATA], file);
		if (sheader.sh_type == SHT_SYMTAB || sheader.sh_type == SHT_DYNSYM)
		{
			get_sh_name(sheader.sh_name, &header, class, file, name);
			size = (class == ELFCLASS32) ?
							I32(sheader.sh_size) / I32(sheader.sh_entsize)
							: I64(sheader.sh_size) / I64(sheader.sh_entsize);
			printf("\nSymbol table '%s' contains %lu entries:\n", name, size);
				printf("   Num:    Value  ");
			if (class == ELFCLASS32)
				printf("Size Type    Bind   Vis      Ndx Name\n");
			else
				printf("        Size Type    Bind   Vis      Ndx Name\n");
			for (j = 0; j < size; j++)
			{
				printf("  %4lu:", j);
				/* load symbol using the linked section header */
				print_symbol_entry(j, &sheader, &header, class, file);
				printf("\n");
			}
		}
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
		fprintf(stderr, "100-hreadelf: Error: Invalid number of arguments.\n");
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "    %s <elf-filename>\n", argv[0]);
		exit(1);
	}
	/* open the given filename */
	file = fopen(argv[1], "rb");
	if (file == NULL)
	{
		fprintf(stderr, "100-hreadelf: Error: '%s': No such file\n", argv[1]);
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
		fprintf(stderr, "100-hreadelf: Error: Not an ELF file");
		fprintf(stderr, " - it has the wrong magic bytes at the start\n");
		fclose(file);
		exit(1);
	}
	/* unknown class elf */
	if (ident[EI_CLASS] != ELFCLASS32 && ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "100-hreadelf: Error: Not a valid ELF file\n");
		fclose(file);
		exit(1);
	}

	print_symtable_info(ident[EI_CLASS], file);

	fclose(file);
	return (0);
}

