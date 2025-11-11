#include "ResponseReceiver.h"
#include <algorithm>
#include <iostream>
#include <cstring>

ResponseReceiver::ResponseReceiver(Communication& comm) : m_comm(comm)
{
	m_responseCode = 0;
	m_payloadSize = 0;
	m_buffer = std::vector<char>(RESPONSE_HEADER_SIZE_BYTES + MAX_UINT_SIZE_BYTES);
	std::fill(std::begin(m_buffer), std::end(m_buffer), 0);
}

void ResponseReceiver::receiveResponse() {
	
	int bytesReceived = recv(m_comm.getSocket(), m_buffer.data(), RESPONSE_HEADER_SIZE_BYTES, 0);

	memcpy_s(&m_responseCode, sizeof(m_responseCode), &m_buffer[VERSION_SIZE_BYTES], RESPONSE_CODE_SIZE_BYTES);
	memcpy_s(&m_payloadSize, sizeof(m_payloadSize), & m_buffer[VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES], PAYLOAD_SIZE_SIZE_BYTES);
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
