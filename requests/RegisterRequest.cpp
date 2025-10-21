#include "RegisterRequest.h"
#include <vector>
#include <iostream>


RegisterRequest::RegisterRequest(std::string username, const uint8_t* publickey) : Request(REGISTER_REQ_CODE, USERNAME_SIZE_BYTES + PUBLICKEY_SIZE_BYTES) {
	uint8_t usernameBytes[USERNAME_SIZE_BYTES] = { 0 };
	std::copy(username.begin(), username.end(), usernameBytes);
	payLoad.insert(payLoad.end(), usernameBytes, usernameBytes + USERNAME_SIZE_BYTES);
	payLoad.insert(payLoad.end(), publickey, publickey + PUBLICKEY_SIZE_BYTES);


}