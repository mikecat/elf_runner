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
	
	return 0;
}
