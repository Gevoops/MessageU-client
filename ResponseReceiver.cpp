#include "ResponseReceiver.h"
#include <algorithm>
#include <iostream>

ResponseReceiver::ResponseReceiver(Communication& comm) : m_comm(comm)
{
	m_responseCode = 0;
	m_payloadSize = 0;
	std::fill(std::begin(m_buffer), std::end(m_buffer), 0);
}

void ResponseReceiver::receiveResponse() {
	
	int bytesReceived = recv(m_comm.getSocket(), m_buffer, RESPONSE_HEADER_SIZE_BYTES, 0);

	std::memcpy(&m_responseCode, &m_buffer[VERSION_SIZE_BYTES], RESPONSE_CODE_SIZE_BYTES);
	std::memcpy(&m_payloadSize, &m_buffer[VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES], PAYLOAD_SIZE_SIZE_BYTES);
	for (uint32_t i = 0; i < m_payloadSize; i += 1024) {
		bytesReceived += recv(m_comm.getSocket(), &m_buffer[RESPONSE_HEADER_SIZE_BYTES + i], 1024, 0);
	}

}


uint16_t ResponseReceiver::getResponseCode() const
{
	return m_responseCode;
}

uint32_t ResponseReceiver::getPayloadSize() const
{
	return m_payloadSize;
}

char* ResponseReceiver::getBuffer()
{
	return &m_buffer[RESPONSE_HEADER_SIZE_BYTES];
}
