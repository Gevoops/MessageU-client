#pragma once
#include <cstdint> 
#include "Constants.h"
#include <winsock2.h>

class Communication {
public:
	Communication();
	SOCKET getSocket() const;
	void initClientId();
private:
	SOCKET initSocket();
	uint8_t clientID[CLIENTID_SIZE_BYTES] = {};
	SOCKET m_socket;
};