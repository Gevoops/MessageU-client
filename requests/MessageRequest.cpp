#include "MessageRequest.h"
#include <vector>
#include <iostream>
#include<winsock2.h>


MessageRequest::MessageRequest(const uint8_t* targetClientID, uint8_t type, int contentSize, const std::vector<uint8_t> &content)
	: Request(MESSAGE_REQ, CLIENTID_SIZE_BYTES + MESSAGE_TYPE_SIZE_BYTES + MESSAGE_CONTENT_SIZE_SIZE_BYTES + content.size()) {

	m_payLoad.clear();
	m_payLoad.insert(m_payLoad.end(), targetClientID, targetClientID + CLIENTID_SIZE_BYTES);
	m_payLoad.push_back(type);

	uint32_t netSize = htonl(contentSize);
	m_payLoad.insert(m_payLoad.end(), 
		reinterpret_cast<uint8_t*>(&netSize),
		reinterpret_cast<uint8_t*>(&netSize) + sizeof(netSize));
	m_payLoad.insert(m_payLoad.end(), content.begin(), content.end());


}