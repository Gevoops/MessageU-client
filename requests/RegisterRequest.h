#pragma once
#include "Request.h"
#include <cstdint>
#include "..\Constants.h"
#include "..\Communication.h"

class RegisterRequest : public Request {
private:
	std::string m_username;
	std::string m_publicKey;

public:
	RegisterRequest(std::string username, std::string publicKey);
};

