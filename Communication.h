#pragma once
#include <cstdint> 
#include "Constants.h"
#include <winsock2.h>

class Communication {
	public:
		static uint8_t clientID[CLIENTID_SIZE_BYTES];

		static SOCKET initSocket();

		static void initClientId();
};