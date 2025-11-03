#pragma once
#include "Request.h"
#include <cstdint>
#include "..\Constants.h"

class MessageRequest : public Request {

public:
	MessageRequest(const uint8_t* targetClientID, uint8_t type, uint32_t contentSize, const std::vector<uint8_t>& content);
};

