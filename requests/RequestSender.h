#pragma once
#include "AllRequests.h"
#include "../Communication.h"
#include <string>
#include "../Contact.h"

class RequestSender {
public:

	RequestSender(Communication& comm);
	void sendRegisterReq(const std::string &username, const uint8_t * publicKey);
	void sendClientListReq();
	void sendPublicKeyReq(Contact * c);
	void sendWaitingMessagesReq();
	void sendMessageReq(const uint8_t* targetClientID, uint8_t type, int contentSize, const std::vector<uint8_t>& content);

private:
	Communication& m_comm;

};

