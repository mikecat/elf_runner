#ifndef FILE_DATA_H_GUARD_1B266614_0DBB_4E51_BF11_DBF4B5CBC7C9
#define FILE_DATA_H_GUARD_1B266614_0DBB_4E51_BF11_DBF4B5CBC7C9

#include <vector>
#include <cstring>
#include <cstdint>
#include <stdexcept>

enum byte_order {
	BO_LITTLE_ENDIAN,
	BO_BIG_ENDIAN
};

class file_data {
	std::vector<uint8_t> data;

public:
	file_data() {}
	file_data(const char* file_name);
	file_data(const void* init_data, size_t size);

	size_t size() const { return data.size(); }
	uint8_t get_u8(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		if (pos >= data.size()) throw std::out_of_range("data index out-of-range");
		switch (bo) {
			case BO_LITTLE_ENDIAN:
			case BO_BIG_ENDIAN:
				return data[pos];
			default:
				throw std::runtime_error("unknown byte order");
		}
	}
	uint16_t get_u16(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		if (data.size() < 2 || pos >= data.size() - 1) throw std::out_of_range("data index out-of-range");
		switch (bo) {
			case BO_LITTLE_ENDIAN:
				return data[pos] | (static_cast<uint16_t>(data[pos + 1]) << 8);
			case BO_BIG_ENDIAN:
				return (static_cast<uint16_t>(data[pos]) << 8) | data[pos + 1];
			default:
				throw std::runtime_error("unknown byte order");
		}
	}
	uint32_t get_u32(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		if (data.size() < 4 || pos >= data.size() - 3) throw std::out_of_range("data index out-of-range");
		uint32_t ret = 0;
		switch (bo) {
			case BO_LITTLE_ENDIAN:
				for (int i = 0; i < 4; i++) {
					ret |= static_cast<uint32_t>(data[pos + i]) << (i * 8);
				}
				break;
			case BO_BIG_ENDIAN:
				for (int i = 0; i < 4; i++) {
					ret |= static_cast<uint32_t>(data[pos + i]) << ((3 - i) * 8);
				}
				break;
			default:
				throw std::runtime_error("unknown byte order");
		}
		return ret;
	}
	uint64_t get_u64(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		if (data.size() < 8 || pos >= data.size() - 7) throw std::out_of_range("data index out-of-range");
		uint64_t ret = 0;
		switch (bo) {
			case BO_LITTLE_ENDIAN:
				for (int i = 0; i < 8; i++) {
					ret |= static_cast<uint64_t>(data[pos + i]) << (i * 8);
				}
				break;
			case BO_BIG_ENDIAN:
				for (int i = 0; i < 8; i++) {
					ret |= static_cast<uint64_t>(data[pos + i]) << ((7 - i) * 8);
				}
				break;
			default:
				throw std::runtime_error("unknown byte order");
		}
		return ret;
	}
	int8_t get_s8(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		uint8_t u = get_u8(pos, bo);
		if (u < INT8_MAX) return static_cast<int8_t>(u);
		else if (u == INT8_C(0x80)) return -INT8_C(0x7f) - 1;
		else return -static_cast<int8_t>(~u) - 1;
	}
	int16_t get_s16(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		uint16_t u = get_u16(pos, bo);
		if (u < INT16_MAX) return static_cast<int16_t>(u);
		else if (u == INT16_C(0x8000)) return -INT16_C(0x7fff) - 1;
		else return -static_cast<int16_t>(~u) - 1;
	}
	int32_t get_s32(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		uint32_t u = get_u32(pos, bo);
		if (u < INT32_MAX) return static_cast<int32_t>(u);
		else if (u == INT32_C(0x80000000)) return -INT32_C(0x7fffffff) - 1;
		else return -static_cast<int32_t>(~u) - 1;
	}
	int64_t get_s64(size_t pos, byte_order bo = BO_LITTLE_ENDIAN) const {
		uint64_t u = get_u64(pos, bo);
		if (u < INT64_MAX) return static_cast<int64_t>(u);
		else if (u == INT64_C(0x8000000000000000)) return -INT64_C(0x7fffffffffffffff) - 1;
		else return -static_cast<int64_t>(~u) - 1;
	}
	void get_bytes(void* out, size_t pos, size_t size) const {
		if (data.size() < size || pos > data.size() - size) throw std::out_of_range("data index out-of-range");
		memcpy(out, &data[pos], size);
	}
};

#endif
