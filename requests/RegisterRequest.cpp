#include "RegisterRequest.h"
#include <vector>
#include <iostream>


RegisterRequest::RegisterRequest(std::string username, const uint8_t* publickey) : Request(REGISTER_REQ, USERNAME_SIZE_BYTES + PUBLICKEY_SIZE_BYTES) {
	uint8_t usernameBytes[USERNAME_SIZE_BYTES] = { 0 };
	std::copy(username.begin(), username.end(), usernameBytes);
	m_payLoad.insert(m_payLoad.end(), usernameBytes, usernameBytes + USERNAME_SIZE_BYTES);
	m_payLoad.insert(m_payLoad.end(), publickey, publickey + PUBLICKEY_SIZE_BYTES);


}