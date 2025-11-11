#include "RegisterRequest.h"
#include <vector>
#include <iostream>
#include "../Constants.h"


RegisterRequest::RegisterRequest(std::string username, std::string publicKey) : Request(REGISTER_REQ, USERNAME_SIZE_BYTES + PUBLIC_KEY_SIZE_BYTES) {
	uint8_t usernameBytes[USERNAME_SIZE_BYTES] = { 0 };
	std::copy(username.begin(), username.end(), usernameBytes);
	m_payLoad.insert(m_payLoad.end(), usernameBytes, usernameBytes + USERNAME_SIZE_BYTES);
	m_payLoad.insert(m_payLoad.end(), publicKey.begin(), publicKey.end());
}
