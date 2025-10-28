#include "RequestSender.h"


RequestSender::RequestSender(Communication& comm) : m_comm(comm)
{
}

void RequestSender::sendRegisterReq(const std::string& username, const uint8_t* publicKey) {
	std::vector<uint8_t> dataVec;
	RegisterRequest request{ username, publicKey };
	dataVec = request.serialize();
	send(m_comm.getSocket(), reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
}

void RequestSender::sendClientListReq() {
	std::vector<uint8_t> dataVec;
	UserListRequest request{};
	dataVec = request.serialize();

	send(m_comm.getSocket(), reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
}

void RequestSender::sendPublicKeyReq(Contact * c) {
	std::vector<uint8_t> dataVec;
	PublicKeyRequest request{ c->getClientID() };
	dataVec = request.serialize();

	send(m_comm.getSocket(), reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
}

void RequestSender::sendWaitingMessagesReq() {
	GetMessagesRequest request{};
	std::vector<uint8_t> dataVec;
	dataVec = request.serialize();
	send(m_comm.getSocket(), reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
}

void RequestSender::sendMessageReq(const uint8_t* targetClientID, uint8_t type, int contentSize, const std::vector<uint8_t>& content) {
	std::vector<uint8_t> dataVec;
	MessageRequest request{targetClientID, type, contentSize, content };
	dataVec = request.serialize();
	send(m_comm.getSocket(), reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);

}
