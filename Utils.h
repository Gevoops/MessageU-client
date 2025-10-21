#pragma once

#include <cstdint> 
#include <string>
class Utils {
	public:
		static const uint8_t* stringToByteHexa(std::string str);
	private:
		Utils() = delete;
};