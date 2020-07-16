#include "elf_data.h"

elf_data elf_data::read_elf(const file_data& data) {
	elf_data result;
	try {
		uint8_t ident[16];
		data.get_bytes(ident, 0, sizeof(ident));
		if (ident[0] != UINT8_C(0x7f) || ident[1] != static_cast<uint8_t>('E') ||
		ident[2] != static_cast<uint8_t>('L') || ident[3] != static_cast<uint8_t>('F')) {
			throw std::runtime_error("not ELF file");
		}
		switch (ident[4]) {
			case 1: result.e_ident_class = 32; break;
			case 2: result.e_ident_class = 64; break;
			default: throw std::runtime_error("invalid or unknown EI_CLASS");
		}
		switch (ident[5]) {
			case 1: result.e_ident_data = BO_LITTLE_ENDIAN; break;
			case 2: result.e_ident_data = BO_BIG_ENDIAN; break;
			default: throw std::runtime_error("invalid or unknown EI_DATA");
		}
		if(ident[6] != 1) {
			throw std::runtime_error("invalid or unknown EI_VERSION");
		}
		result.e_ident_version = ident[6];

		result.e_type = data.get_u16(0x10, result.e_ident_data);
		result.e_machine = data.get_u16(0x12, result.e_ident_data);
		result.e_version = data.get_u32(0x14, result.e_ident_data);
		size_t e_flags_offset;
		if (result.e_ident_class == 64) {
			result.e_entry = data.get_u64(0x18, result.e_ident_data);
			result.e_phoff = data.get_u64(0x20, result.e_ident_data);
			result.e_shoff = data.get_u64(0x28, result.e_ident_data);
			e_flags_offset = 0x30;
		} else {
			result.e_entry = data.get_u32(0x18, result.e_ident_data);
			result.e_phoff = data.get_u32(0x1C, result.e_ident_data);
			result.e_shoff = data.get_u32(0x20, result.e_ident_data);
			e_flags_offset = 0x24;
		}
		result.e_flags = data.get_u32(e_flags_offset, result.e_ident_data);
		result.e_ehsize = data.get_u16(e_flags_offset + 4, result.e_ident_data);
		result.e_phentsize = data.get_u16(e_flags_offset + 6, result.e_ident_data);
		result.e_phnum = data.get_u16(e_flags_offset + 8, result.e_ident_data);
		result.e_shentsize = data.get_u16(e_flags_offset + 10, result.e_ident_data);
		result.e_shnum = data.get_u16(e_flags_offset + 12, result.e_ident_data);
		result.e_shstrndx = data.get_u16(e_flags_offset + 14, result.e_ident_data);

		if (result.e_phentsize < (result.e_ident_class == 64 ? 56 : 32)) {
			throw std::runtime_error("program header too small");
		}
		for (uint16_t i = 0; i < result.e_phnum; i++) {
			size_t entry_offset = result.e_phoff + static_cast<size_t>(i) * result.e_phentsize;
			program_header_data entry;
			entry.p_type = data.get_u32(entry_offset, result.e_ident_data);
			if (result.e_ident_class == 64) {
				entry.p_offset = data.get_u64(entry_offset + 0x08, result.e_ident_data);
				entry.p_vaddr = data.get_u64(entry_offset + 0x10, result.e_ident_data);
				entry.p_paddr = data.get_u64(entry_offset + 0x18, result.e_ident_data);
				entry.p_filesz = data.get_u64(entry_offset + 0x20, result.e_ident_data);
				entry.p_memsz = data.get_u64(entry_offset + 0x28, result.e_ident_data);
				entry.p_flags = data.get_u32(entry_offset + 0x04, result.e_ident_data);
				entry.p_align = data.get_u64(entry_offset + 0x30, result.e_ident_data);
			} else {
				entry.p_offset = data.get_u32(entry_offset + 0x04, result.e_ident_data);
				entry.p_vaddr = data.get_u32(entry_offset + 0x08, result.e_ident_data);
				entry.p_paddr = data.get_u32(entry_offset + 0x0C, result.e_ident_data);
				entry.p_filesz = data.get_u32(entry_offset + 0x10, result.e_ident_data);
				entry.p_memsz = data.get_u32(entry_offset + 0x14, result.e_ident_data);
				entry.p_flags = data.get_u32(entry_offset + 0x18, result.e_ident_data);
				entry.p_align = data.get_u32(entry_offset + 0x1C, result.e_ident_data);
			}
			entry.data.resize(entry.p_filesz);
			data.get_bytes(&entry.data[0], entry.p_offset, entry.p_filesz);
			result.ph.push_back(entry);
		}
	} catch (std::out_of_range&) {
		throw std::runtime_error("file too small");
	}
	return result;
}
