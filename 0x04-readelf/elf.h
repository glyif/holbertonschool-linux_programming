#ifndef ELF_H
#define ELF_H

#include <stdlib.h>
#include <stdint.h>

#define	ELF_NIDENT	16

#define	EI_MAG0			0
#define	EI_MAG1			1
#define	EI_MAG2			2
#define	EI_MAG3			3
#define	EI_CLASS		4
#define	EI_DATA			5
#define	EI_VERSION		6
#define	EI_OSABI		7
#define	EI_ABIVERSION	8
#define	EI_PAD			9

#define	ELFMAG0		0x7f
#define	ELFMAG1		'E'
#define	ELFMAG2		'L'
#define	ELFMAG3		'F'

#define	ELFCLASSNONE	0
#define	ELFCLASS32		1
#define	ELFCLASS64		2

#define	ELFDATANONE	0
#define	ELFDATA2LSB	1
#define	ELFDATA2MSB	2

#define ELFOSABI_NONE			0
#define ELFOSABI_HPUX			1
#define ELFOSABI_NETBSD			2
#define ELFOSABI_GNU			3
#define ELFOSABI_LINUX			3
#define ELFOSABI_HURD			4
#define ELFOSABI_SOLARIS		6
#define ELFOSABI_AIX			7
#define ELFOSABI_IRIX			8
#define ELFOSABI_FREEBSD		9
#define ELFOSABI_TRU64			10
#define ELFOSABI_MODESTO		11
#define ELFOSABI_OPENBSD		12
#define ELFOSABI_OPENVMS		13
#define ELFOSABI_NSK			14
#define ELFOSABI_AROS			15
#define ELFOSABI_FENIXOS		16
#define ELFOSABI_CLOUDABI		17
#define ELFOSABI_OPENVOS		18
#define ELFOSABI_C6000_ELFABI   64
#define ELFOSABI_C6000_LINUX    65
#define ELFOSABI_ARM			97
#define ELFOSABI_STANDALONE		255

#define ET_NONE		0
#define ET_REL		1
#define ET_EXEC		2
#define ET_DYN		3
#define ET_CORE		4
#define ET_LOOS		0xfe00
#define ET_HIOS		0xfeff
#define ET_LOPROC	0xff00
#define ET_HIPROC	0xffff

#define EM_NONE				0
#define EM_M32				1
#define EM_SPARC			2
#define EM_386				3
#define EM_68K				4
#define EM_88K				5
#define EM_IAMCU			6
#define EM_860				7
#define EM_MIPS				8
#define EM_S370				9
#define EM_MIPS_RS3_LE		10
#define EM_PARISC			15
#define EM_VPP500			17
#define EM_SPARC32PLUS		18
#define EM_960				19
#define EM_PPC				20
#define EM_PPC64			21
#define EM_S390				22
#define EM_SPU				23
#define EM_V800				36
#define EM_FR20				37
#define EM_RH32				38
#define EM_RCE				39
#define EM_ARM				40
#define EM_FAKE_ALPHA		41
#define EM_SH				42
#define EM_SPARCV9			43
#define EM_TRICORE			44
#define EM_ARC				45
#define EM_H8_300			46
#define EM_H8_300H			47
#define EM_H8S				48
#define EM_H8_500			49
#define EM_IA_64			50
#define EM_MIPS_X			51
#define EM_COLDFIRE			52
#define EM_68HC12			53
#define EM_MMA				54
#define EM_PCP				55
#define EM_NCPU				56
#define EM_NDR1				57
#define EM_STARCORE			58
#define EM_ME16				59
#define EM_ST100			60
#define EM_TINYJ			61
#define EM_X86_64			62
#define EM_PDSP				63
#define EM_PDP10			64
#define EM_PDP11			65
#define EM_FX66				66
#define EM_ST9PLUS			67
#define EM_ST7				68
#define EM_68HC16			69
#define EM_68HC11			70
#define EM_68HC08			71
#define EM_68HC05			72
#define EM_SVX				73
#define EM_ST19				74
#define EM_VAX				75
#define EM_CRIS				76
#define EM_JAVELIN			77
#define EM_FIREPATH			78
#define EM_ZSP				79
#define EM_MMIX				80
#define EM_HUANY			81
#define EM_PRISM			82
#define EM_AVR				83
#define EM_FR30				84
#define EM_D10V				85
#define EM_D30V				86
#define EM_V850				87
#define EM_M32R				88
#define EM_MN10300			89
#define EM_MN10200			90
#define EM_PJ				91
#define EM_OPENRISC			92
#define EM_ARC_COMPACT		93
#define EM_XTENSA			94
#define EM_VIDEOCORE		95
#define EM_TMM_GPP			96
#define EM_NS32K			97
#define EM_TPC				98
#define EM_SNP1K			99
#define EM_ST200			100
#define EM_IP2K				101
#define EM_MAX				102
#define EM_CR				103
#define EM_F2MC16			104
#define EM_MSP430			105
#define EM_BLACKFIN			106
#define EM_SE_C33			107
#define EM_SEP				108
#define EM_ARCA				109
#define EM_UNICORE			110
#define EM_EXCESS			111
#define EM_DXP				112
#define EM_ALTERA_NIOS2		113
#define EM_CRX				114
#define EM_XGATE			115
#define EM_C166				116
#define EM_M16C				117
#define EM_DSPIC30F			118
#define EM_CE				119
#define EM_M32C				120
#define EM_TSK3000			131
#define EM_RS08				132
#define EM_SHARC			133
#define EM_ECOG2			134
#define EM_SCORE7			135
#define EM_DSP24			136
#define EM_VIDEOCORE3		137
#define EM_LATTICEMICO32	138
#define EM_SE_C17			139
#define EM_TI_C6000			140
#define EM_TI_C2000			141
#define EM_TI_C5500			142
#define EM_TI_ARP32			143
#define EM_TI_PRU			144
#define EM_MMDSP_PLUS		160
#define EM_MMDSP_PLUS		160
#define EM_CYPRESS_M8C		161
#define EM_R32C				162
#define EM_TRIMEDIA			163
#define EM_QDSP6			164
#define EM_8051				165
#define EM_STXP7X			166
#define EM_NDS32			167
#define EM_ECOG1			168
#define EM_MAXQ30			169
#define EM_XIMO16			170
#define EM_MANIK			171
#define EM_CRAYNV2			172
#define EM_RX				173
#define EM_METAG			174
#define EM_MCST_ELBRUS		175
#define EM_ECOG16			176
#define EM_CR16				177
#define EM_ETPU				178
#define EM_SLE9X			179
#define EM_L10M				180
#define EM_K10M				181
#define EM_AARCH64			183
#define EM_AVR32			185
#define EM_STM8				186
#define EM_TILE64			187
#define EM_TILEPRO			188
#define EM_MICROBLAZE		189
#define EM_CUDA				190
#define EM_TILEGX			191
#define EM_CLOUDSHIELD		192
#define EM_COREA_1ST		193
#define EM_COREA_2ND		194
#define EM_ARC_COMPACT2		195
#define EM_OPEN8			196
#define EM_RL78				197
#define EM_VIDEOCORE5		198
#define EM_78KOR			199
#define EM_56800EX			200
#define EM_BA1				201
#define EM_BA2				202
#define EM_XCORE			203
#define EM_MCHP_PIC			204
#define EM_INTEL205			205
#define EM_INTEL206			206
#define EM_INTEL207			207
#define EM_INTEL208			208
#define EM_INTEL209			209
#define EM_KM32				210
#define EM_KMX32			211
#define EM_KMX16			212
#define EM_KMX8				213
#define EM_KVARC			214
#define EM_CDP				215
#define EM_COGE				216
#define EM_COOL				217
#define EM_NORC				218
#define EM_CSR_KALIMBA		219
#define EM_Z80				220
#define EM_VISIUM			221
#define EM_FT32				222
#define EM_MOXIE			223
#define EM_AMDGPU			224
#define EM_RISCV			243
#define EM_ALPHA			0x9026

#define SHT_NULL			0
#define SHT_PROGBITS		1
#define SHT_SYMTAB			2
#define SHT_STRTAB			3
#define SHT_RELA			4
#define SHT_HASH			5
#define SHT_DYNAMIC			6
#define SHT_NOTE			7
#define SHT_NOBITS			8
#define SHT_REL				9
#define SHT_SHLIB			10
#define SHT_DYNSYM			11
#define SHT_INIT_ARRAY		14
#define SHT_FINI_ARRAY		15
#define SHT_PREINIT_ARRAY	16
#define SHT_GROUP			17
#define SHT_SYMTAB_SHNDX	18
#define SHT_LOOS			0x60000000
#define SHT_GNU_ATTRIBUTES	0x6ffffff5
#define SHT_GNU_HASH		0x6ffffff6
#define SHT_GNU_verdef		0x6ffffffd
#define SHT_GNU_verneed		0x6ffffffe
#define SHT_GNU_versym		0x6fffffff
#define SHT_HIOS			0x6fffffff
#define SHT_LOPROC			0x70000000
#define SHT_HIPROC			0x7fffffff
#define SHT_LOUSER			0x80000000
#define SHT_HIUSER			0xffffffff
#define SHF_X86_64_LARGE	0x10000000

#define SHF_WRITE				0x1
#define SHF_ALLOC				0x2
#define SHF_EXECINSTR			0x4
#define SHF_MERGE				0x10
#define SHF_STRINGS				0x20
#define SHF_INFO_LINK			0x40
#define SHF_LINK_ORDER			0x80
#define SHF_OS_NONCONFORMING	0x100
#define SHF_GROUP				0x200
#define SHF_TLS					0x400
#define SHF_EXCLUDE				0x80000000
#define SHF_COMPRESSED			0x800
#define SHF_MASKOS				0x0ff00000
#define SHF_MASKPROC			0xf0000000

#define PT_NULL				0
#define PT_LOAD				1
#define PT_DYNAMIC			2
#define PT_INTERP			3
#define PT_NOTE				4
#define PT_SHLIB			5
#define PT_PHDR				6
#define PT_TLS				7
#define	PT_NUM				8
#define PT_LOOS				0x60000000
#define PT_GNU_EH_FRAME		0x6474e550
#define PT_GNU_STACK		0x6474e551
#define PT_GNU_RELRO		0x6474e552
#define PT_LOSUNW			0x6ffffffa
#define PT_SUNWBSS			0x6ffffffa
#define PT_SUNWSTACK		0x6ffffffb
#define PT_HIOS				0x6fffffff
#define PT_LOPROC			0x70000000
#define PT_HIPROC			0x7fffffff

#define PF_X				1
#define PF_W				2
#define PF_R				4
#define PF_MASKOS			0x0ff00000
#define PF_MASKPROC			0xf0000000

#define STT_NOTYPE			0
#define STT_OBJECT			1
#define STT_FUNC			2
#define STT_SECTION			3
#define STT_FILE			4
#define STT_COMMON			5
#define STT_LOOS			10
#define STT_HIOS			12
#define STT_LOPROC			13
#define STT_HIPROC			15

#define STB_LOCAL			0
#define STB_GLOBAL			1
#define STB_WEAK			2
#define STB_LOOS			10
#define STB_HIOS			12
#define STB_LOPROC			13
#define STB_HIPROC			15

#define STV_DEFAULT			0
#define STV_INTERNAL		1
#define STV_HIDDEN			2
#define STV_PROTECTED		3

#define SHN_UNDEF			0
#define SHN_LORESERVE		0xff00
#define SHN_LOPROC			0xff00
#define SHN_HIPROC			0xff1f
#define SHN_LOOS			0xff20
#define SHN_HIOS			0xff3f
#define SHN_ABS				0xfff1
#define SHN_COMMON			0xfff2
#define SHN_XINDEX			0xffff
#define SHN_HIRESERVE		0xffff

/**
 * Elf32_Ehdr - elf32 header struct
 * @e_ident" indent
 * @e_type: type
 * @e_machine: machine type
 * @e_version: version
 * @e_entry: entry offset
 * @e_phoff: phoff
 * @e_shoff: shoff
 * @e_flags: flags
 * @e_ehsize: ehsize
 * @e_phentsize: phentsize
 * @e_phnum; phum
 * @e_shentsize; shentsize
 * @e_shnum; shnum
 * @e_shstrndx; shstrndx
 */
typedef struct
{
	uint8_t  e_ident[ELF_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint32_t e_entry;
	uint32_t e_phoff;
	uint32_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} Elf32_Ehdr;

/**
 * Elf64_Ehdr - elf 64 bit header
 * @e_ident" indent
 * @e_type: type
 * @e_machine: machine type
 * @e_version: version
 * @e_entry: entry offset
 * @e_phoff: phoff
 * @e_shoff: shoff
 * @e_flags: flags
 * @e_ehsize: ehsize
 * @e_phentsize: phentsize
 * @e_phnum; phum
 * @e_shentsize; shentsize
 * @e_shnum; shnum
 * @e_shstrndx; shstrndx
 */
typedef struct
{
	uint8_t  e_ident[ELF_NIDENT];
	uint16_t e_type;
	uint16_t e_machine;
	uint32_t e_version;
	uint64_t e_entry;
	uint64_t e_phoff;
	uint64_t e_shoff;
	uint32_t e_flags;
	uint16_t e_ehsize;
	uint16_t e_phentsize;
	uint16_t e_phnum;
	uint16_t e_shentsize;
	uint16_t e_shnum;
	uint16_t e_shstrndx;
} Elf64_Ehdr;

typedef struct
{
	uint32_t sh_name;
	uint32_t sh_type;
	uint32_t sh_flags;
	uint32_t sh_addr;
	uint32_t sh_offset;
	uint32_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint32_t sh_addralign;
	uint32_t sh_entsize;
} Elf32_Shdr;

typedef struct
{
	uint32_t sh_name;
	uint32_t sh_type;
	uint64_t sh_flags;
	uint64_t sh_addr;
	uint64_t sh_offset;
	uint64_t sh_size;
	uint32_t sh_link;
	uint32_t sh_info;
	uint64_t sh_addralign;
	uint64_t sh_entsize;
} Elf64_Shdr;

typedef struct
{
	uint8_t		*e_ident;
	uint16_t	e_type;
	uint16_t	e_machine;
	uint32_t	e_version;
	void		*e_entry;
	void		*e_phoff;
	void		*e_shoff;
	uint32_t	e_flags;
	uint16_t	e_ehsize;
	uint16_t	e_phentsize;
	uint16_t	e_phnum;
	uint16_t	e_shentsize;
	uint16_t	e_shnum;
	uint16_t	e_shstrndx;
	uint8_t		data[64];
} Elf_Ehdr;

typedef struct
{
	uint32_t	sh_name;
	uint32_t	sh_type;
	void		*sh_flags;
	void		*sh_addr;
	void		*sh_offset;
	void		*sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	void		*sh_addralign;
	void		*sh_entsize;
	uint8_t		data[64];
} Elf_Shdr;

typedef struct
{
	uint32_t	p_type;
	uint32_t	p_offset;
	uint32_t	p_vaddr;
	uint32_t	p_paddr;
	uint32_t	p_filesz;
	uint32_t	p_memsz;
	uint32_t	p_flags;
	uint32_t	p_align;
} Elf32_Phdr;

typedef struct
{
	uint32_t	p_type;
	uint32_t	p_flags;
	uint64_t	p_offset;
	uint64_t	p_vaddr;
	uint64_t	p_paddr;
	uint64_t	p_filesz;
	uint64_t	p_memsz;
	uint64_t	p_align;
} Elf64_Phdr;

typedef struct
{
	uint32_t	p_type;
	void		*p_offset;
	void		*p_vaddr;
	void		*p_paddr;
	void		*p_filesz;
	void		*p_memsz;
	uint32_t	p_flags;
	void		*p_align;
	uint8_t		data[64];
} Elf_Phdr;

typedef struct
{
	uint32_t	st_name;
	uint32_t	st_value;
	uint32_t	st_size;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shndx;
} Elf32_Sym;

typedef struct
{
	uint32_t	st_name;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shndx;
	uint64_t	st_value;
	uint64_t	st_size;
} Elf64_Sym;

typedef struct
{
	uint32_t	st_name;
	void		*st_value;
	void		*st_size;
	uint8_t		st_info;
	uint8_t		st_other;
	uint16_t	st_shndx;
	uint8_t		data[32];
} Elf_Sym;

typedef struct
{
	uint16_t	vn_version;
	uint16_t	vn_cnt;
	uint32_t	vn_file;
	uint32_t	vn_aux;
	uint32_t	vn_next;
} Elf_Verneed;

typedef struct
{
	uint32_t	vna_hash;
	uint16_t	vna_flags;
	uint16_t	vna_other;
	uint32_t	vna_name;
	uint32_t	vna_next;
} Elf_Vernaux;

#define I32(X) ((uint32_t *)(X))[0]
#define I64(X) ((uint64_t *)(X))[0]

#define ST_BIND(i)   ((i) >> 4)
#define ST_TYPE(i)   ((i) & 0xf)
#define ST_VISIBILITY(o) ((o) & 0x3)

#define MAX_NAME_LEN	100
typedef struct
{
	int		id;
	char	name[MAX_NAME_LEN];
} translation_table_t;

#endif /* ELF_H */

