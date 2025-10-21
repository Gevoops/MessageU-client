#include "Request.h"
#include "../Constants.h"
#include "../Communication.h"
#include "../Utils.h"
#include <fstream>
#include <iostream>
#include <string>

uint8_t Request::version = VERSION;
Request::Request(uint16_t code, uint32_t sizeOfPayload)
: requestCode(code), payloadSize(sizeOfPayload) {
	std::memcpy(clientID, Communication::clientID, CLIENTID_SIZE_BYTES);
}


const uint8_t* Request::getClientID() const {
	return clientID;
}

uint16_t Request::getRequestCode() const {
	return requestCode;
}

uint32_t Request::getpayLoadSize() const {
	return payloadSize;
}

std::vector<uint8_t> Request::serialize() const {
	std::vector<uint8_t> dataVec;
	dataVec.assign(this->clientID, this->clientID + sizeof(clientID));

	dataVec.insert(dataVec.end(), version);
	size_t startPos = dataVec.size();
	uint16_t reqCodeData = this->requestCode;
	dataVec.resize(startPos + sizeof(reqCodeData));
	std::memcpy(dataVec.data() + startPos, &reqCodeData, sizeof(reqCodeData));
	startPos = dataVec.size();
	uint32_t sizeOfPayload = this->payloadSize;
	dataVec.resize(startPos + sizeof(sizeOfPayload));
	std::memcpy(dataVec.data() + startPos, &sizeOfPayload, sizeof(sizeOfPayload));
	
	dataVec.insert(dataVec.end(), payLoad.begin(), payLoad.end());

	return dataVec;
}
