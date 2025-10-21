#pragma once
#include "Request.h"
#include <cstdint>
#include "..\Constants.h"

class MessageRequest : public Request {

public:
	MessageRequest(const uint8_t* targetClientID, uint8_t type, int contentSize, const std::vector<uint8_t>& content);
};

