#include "Utils.h"

const uint8_t* Utils::stringToByteHexa(std::string str) {
	uint8_t result[sizeof(str) / 2];
	for (int i = 0; i < str.length(); i += 2) {
		std::string byte_hexa = str.substr(i, 2);
		uint8_t byte_int = static_cast<int8_t>(std::strtol(byte_hexa.c_str(), nullptr, 16)); // 16 = hexadecimal (base 16)
		result[i / 2] = byte_int;
	}
	return result;
}