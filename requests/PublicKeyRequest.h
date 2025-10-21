#pragma once
#include "Request.h"
#include <cstdint>
#include "..\Constants.h"

class PublicKeyRequest : public Request {

public:
	PublicKeyRequest(const uint8_t* targetClientID);
};

