#include "PublicKeyRequest.h"
#include <vector>
#include <iostream>


PublicKeyRequest::PublicKeyRequest(const uint8_t* targetClientID) : Request(PUB_KEY_REQ, CLIENTID_SIZE_BYTES) {
	payLoad.insert(payLoad.end(), targetClientID, targetClientID + CLIENTID_SIZE_BYTES);
	
}