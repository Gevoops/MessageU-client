#pragma once
#include <cstdint> 
#include "Constants.h"
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class Communication {
public:
	Communication();
	SOCKET getSocket();
	static bool isLittleEndian();
	static uint32_t swapEndian32(uint32_t num);
	static uint16_t swapEndian16(uint16_t num);
private:
	void initClientId();
	SOCKET initSocket();
	uint8_t clientID[CLIENTID_SIZE_BYTES] = {};
	SOCKET m_socket;
};