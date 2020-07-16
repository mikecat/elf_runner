#include <iostream>
#include <iomanip>
#include <string>
#include "file_data.h"
#include "elf_data.h"

std::string get_byte_order(byte_order bo) {
	switch (bo) {
		case BO_LITTLE_ENDIAN: return "BO_LITTLE_ENDIAN";
		case BO_BIG_ENDIAN: return "BO_BIG_ENDIAN";
		default: return "(unknown)";
	}
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cerr << "Usage: " << (argc > 1 ? argv[0] : "elf_reader") << " input_file" << std::endl;
		return 1;
	}
	elf_data elf;
	try {
		file_data input_data(argv[1]);
		elf = elf_data::read_elf(input_data);
	} catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
		return 1;
	}

	std::cout << "e_ident_class   : " << elf.e_ident_class << '\n';
	std::cout << "e_ident_data    : " << get_byte_order(elf.e_ident_data) << '\n';
	std::cout << "e_ident_version : " << elf.e_ident_version << '\n';
	std::cout << '\n';
	std::cout << "e_type    : " << elf.e_type << '\n';
	std::cout << "e_machine : " << elf.e_machine << '\n';
	std::cout << "e_version : " << elf.e_version << '\n';
	std::cout << '\n';
	std::cout << std::hex;
	std::cout << "e_entry : 0x" << elf.e_entry << '\n';
	std::cout << "e_phoff : 0x" << elf.e_phoff << '\n';
	std::cout << "e_shoff : 0x" << elf.e_shoff << '\n';
	std::cout << "e_flags : 0x" << std::setw(8) << std::setfill('0') << elf.e_flags << '\n';
	std::cout << std::dec;
	std::cout << '\n';
	std::cout << "e_ehsize    : " << elf.e_ehsize << '\n';
	std::cout << "e_phentsize : " << elf.e_phentsize << '\n';
	std::cout << "e_phnum     : " << elf.e_phnum << '\n';
	std::cout << "e_shentsize : " << elf.e_shentsize << '\n';
	std::cout << "e_shnum     : " << elf.e_shnum << '\n';
	std::cout << "e_shstrndx  : " << elf.e_shstrndx << '\n';

	std::cout << "\n----------------- program headers -----------------\n";
	for (size_t i = 0; i < elf.ph.size(); i++) {
		std::cout << "\n----- ph[" << i << "] -----\n";
		std::cout << std::hex;
		std::cout << "p_type   : 0x" << elf.ph[i].p_type << '\n';
		std::cout << "p_offset : 0x" << elf.ph[i].p_offset << '\n';
		std::cout << "p_vaddr  : 0x" << elf.ph[i].p_vaddr << '\n';
		std::cout << "p_paddr  : 0x" << elf.ph[i].p_paddr << '\n';
		std::cout << "p_filesz : 0x" << elf.ph[i].p_filesz << '\n';
		std::cout << "p_memsz  : 0x" << elf.ph[i].p_memsz << '\n';
		std::cout << "p_flags  : 0x" << elf.ph[i].p_flags << '\n';
		std::cout << "p_align  : 0x" << elf.ph[i].p_align << '\n';
		std::cout << std::dec;
	}

	std::cout << "\n----------------- section headers -----------------\n";
	for (size_t i = 0; i < elf.sh.size(); i++) {
		std::cout << "\n----- sh[" << i << "] -----\n";
		std::cout << std::hex;
		std::cout << "sh_name      : 0x" << elf.sh[i].sh_name << " (" << elf.sh[i].name << ")\n";
		std::cout << "sh_type      : 0x" << elf.sh[i].sh_type << '\n';
		std::cout << "sh_flags     : 0x" << elf.sh[i].sh_flags << '\n';
		std::cout << "sh_addr      : 0x" << elf.sh[i].sh_addr << '\n';
		std::cout << "sh_offset    : 0x" << elf.sh[i].sh_offset << '\n';
		std::cout << "sh_size      : 0x" << elf.sh[i].sh_size << '\n';
		std::cout << "sh_link      : 0x" << elf.sh[i].sh_link << '\n';
		std::cout << "sh_info      : 0x" << elf.sh[i].sh_info << '\n';
		std::cout << "sh_addralign : 0x" << elf.sh[i].sh_addralign << '\n';
		std::cout << "sh_entsize   : 0x" << elf.sh[i].sh_entsize << '\n';
		std::cout << std::dec;
	}

	return 0;
}
