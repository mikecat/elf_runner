#include <cstdio>#include "file_data.h"file_data::file_data(const char* file_name) {	FILE* fp = fopen(file_name, "rb");	constexpr int BUFFER_SIZE = 4096;	uint8_t buffer[BUFFER_SIZE];	if (fp == NULL) {		throw std::runtime_error("file open error");	}	for (;;) {		size_t size = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, fp);		if (ferror(fp)) {			fclose(fp);			throw std::runtime_error("file read error");		} else if (size == 0) {			break;		} else {			size_t current_size = data.size();			data.resize(data.size() + size);			memcpy(&data[current_size], buffer, size);		}	}}file_data::file_data(const void* init_data, size_t size) {	data.resize(size);	memcpy(&data[0], init_data, size);}