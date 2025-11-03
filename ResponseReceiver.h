#pragma once
#include "Communication.h"

class ResponseReceiver {
public:
	ResponseReceiver(Communication& comm);
	void receiveResponse();
	uint16_t getResponseCode() const;
	uint32_t getPayloadSize() const;
	char* getBuffer();

private:
	uint16_t m_responseCode = 0;
	uint32_t m_payloadSize = 0;
	char m_buffer[1024 * 50];
	Communication &m_comm;

	static const int RESPONSE_CODE_SIZE_BYTES = 2;
	static const int PAYLOAD_SIZE_SIZE_BYTES = 4;
	static const int RESPONSE_HEADER_SIZE_BYTES = VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES + PAYLOAD_SIZE_SIZE_BYTES;
	static const int MESSAGE_CLIENTID_OFFSET = RESPONSE_HEADER_SIZE_BYTES;
	static const int MESSAGE_ID_OFFSET = RESPONSE_HEADER_SIZE_BYTES + CLIENTID_SIZE_BYTES;
	static const int MESSAGE_TYPE_OFFSET = MESSAGE_ID_OFFSET + MESSAGE_ID_SIZE_BYTES;
	static const int MESSAGE_CONTENT_SIZE_OFFSET = MESSAGE_TYPE_OFFSET + MESSAGE_TYPE_SIZE_BYTES;
	static const int MESSAGE_CONTENT_OFFSET = MESSAGE_CONTENT_SIZE_OFFSET + MESSAGE_CONTENT_SIZE_SIZE_BYTES;
};