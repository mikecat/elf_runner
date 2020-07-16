#ifndef ELF_DATA_H_GUARD_DA228AC2_D595_40A5_8E0C_465FB43CD438
#define ELF_DATA_H_GUARD_DA228AC2_D595_40A5_8E0C_465FB43CD438

#include <vector>
#include <cstdint>
#include "file_data.h"

struct program_header_data {
	uint32_t p_type;
	uint32_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint32_t p_flags;
	uint64_t p_align;
	std::vector<uint8_t> data;
};

struct elf_data {
	int e_ident_class;
	byte_order e_ident_data;
	int e_ident_version;

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

	std::vector<program_header_data> ph;

	static elf_data read_elf(const file_data& data);
};

#endif
