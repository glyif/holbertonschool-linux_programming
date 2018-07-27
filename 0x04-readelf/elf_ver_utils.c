#include "elf_ver_utils.h"

/**
 *	loads an Elf verneed from the file and saves it in the generic Elf
 *	verneed structure hdr, the file is assumed to be in the given class
 */
void load_verneed(Elf_Verneed *hdr, uint8_t data, FILE *file)
{
	fread(hdr, sizeof(Elf_Verneed), 1, file);   /* read the data */
	if (data == ELFDATA2MSB)
	{
		big_to_little((void *)&hdr->vn_version, 16);
		big_to_little((void *)&hdr->vn_cnt, 16);
		big_to_little((void *)&hdr->vn_file, 32);
		big_to_little((void *)&hdr->vn_aux, 32);
		big_to_little((void *)&hdr->vn_next, 32);
	}
}

/**
 *	loads an Elf vernaux from the file and saves it in the generic Elf
 *	vernaux structure hdr, the file is assumed to be in the given class
 */
void load_vernaux(Elf_Vernaux *hdr, uint8_t data, FILE *file)
{
	fread(hdr, sizeof(Elf_Vernaux), 1, file);   /* read the data */
	if (data == ELFDATA2MSB)
	{
		big_to_little((void *)&hdr->vna_hash, 32);
		big_to_little((void *)&hdr->vna_flags, 16);
		big_to_little((void *)&hdr->vna_other, 16);
		big_to_little((void *)&hdr->vna_name, 32);
		big_to_little((void *)&hdr->vna_next, 32);
	}
}

