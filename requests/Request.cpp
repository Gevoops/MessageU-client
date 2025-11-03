#include "Request.h"
#include "../Constants.h"
#include "../Communication.h"
#include "../Utils.h"
#include <fstream>
#include <iostream>
#include <string>
#include "../FileHandler.h"

uint8_t Request::version = VERSION;
Request::Request(uint16_t code, uint32_t sizeOfPayload)
: m_requestCode(code), m_payloadSize(sizeOfPayload) {
	FileHandler::readClientID(m_clientID);
}

uint16_t Request::getRequestCode() const {
	return m_requestCode;
}

uint32_t Request::getpayLoadSize() const {
	return m_payloadSize;
}

std::vector<uint8_t> Request::serialize() const {
	uint16_t reqCodeData = this->m_requestCode;
	uint32_t sizeOfPayload = this->m_payloadSize;
	if (!Communication::isLittleEndian()) {
		reqCodeData = Communication::swapEndian16(reqCodeData);
		sizeOfPayload = Communication::swapEndian32(sizeOfPayload);
	}
	std::vector<uint8_t> dataVec;
	dataVec.assign(this->m_clientID,this->m_clientID + sizeof(m_clientID));
	dataVec.insert(dataVec.end(), version);
	size_t startPos = dataVec.size();
	dataVec.resize(startPos + sizeof(reqCodeData));
	std::memcpy(dataVec.data() + startPos, &reqCodeData, sizeof(reqCodeData));
	startPos = dataVec.size();
	dataVec.resize(startPos + sizeof(sizeOfPayload));
	std::memcpy(dataVec.data() + startPos, &sizeOfPayload, sizeof(sizeOfPayload));
	dataVec.insert(dataVec.end(), m_payLoad.begin(), m_payLoad.end());

	return dataVec;
}

void Request::setClientID()
{
}
