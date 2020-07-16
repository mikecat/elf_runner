CXX=g++
LD=g++
CXXFLAGS=-Wall -Wextra -pedantic -std=c++11
LDFLAGS=-O2 -s -static

.PHONY: all
all: elf_reader

elf_reader: elf_data.o elf_reader.o file_data.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
