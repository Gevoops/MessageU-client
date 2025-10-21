#pragma once
#include "Request.h"
#include <cstdint>
#include "..\Constants.h"

class RegisterRequest : public Request {

public:
	RegisterRequest(std::string username, const uint8_t* publickey);
};

