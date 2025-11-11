#pragma once
#include "FileHandler.h"
#include "Communication.h"
#include "ResponseReceiver.h"
#include "Constants.h"


class ClientController {
public:
	ClientController(FileHandler& fileHandler, ResponseReceiver & receiver, Communication &comm);
	void run();
	


private:
	std::string m_registrationUsername = "";
	FileHandler& m_fileHandler;
	std::string m_privKey;
	ResponseReceiver& m_receiver;
	Communication& m_comm;

	void printMenu();
	bool handleChoice(int choice);
	int readInput();
	std::string inputUsername();
	void handleResponse();
	bool registerClient();
	bool getClientList();
	bool reqPublicKey();
	bool getWaitingMessages();
	bool sendTextMessage();
	bool reqSymmKey();
	bool sendSymmKey();

	void regSuccess(char * buffer);
	void userListResponse(char* buffer, uint32_t payloadSize);
	void publicKeyResponse(char* buffer);
	void messageReceivedByServer(char* buffer);
	void waitingMessages(char* buffer, uint32_t payloadSize);

	

	static constexpr int INVALID_INPUT = -1;
	static constexpr int REGISTER = 110;
	static constexpr int REQ_CLIENT_LIST = 120;
	static constexpr int REQ_PUBLIC_KEY = 130;
	static constexpr int REQ_WAITING_MESSAGES = 140;
	static constexpr int SEND_TXT_MESSAGE = 150;
	static constexpr int SEND_REQ_FOR_SYMM_KEY = 151;
	static constexpr int SEND_SYMM_KEY = 152;
	static constexpr int EXIT = 0;

	// requests 
	static constexpr int REQ_CODE_DIGITS = 3;


	


	// response 
	static constexpr int REG_SUCCESS_RESPONSE = 2100;
	static constexpr int USER_LIST_RESPONSE = 2101;
	static constexpr int PUBLIC_KEY_RESPONSE = 2102;
	static constexpr int MESSAGE_SENT_RESPONSE = 2103;
	static constexpr int WAITING_MESSAGES_RESPONSE = 2104;
	static constexpr int SERVER_ERROR_RESPONSE = 9000;
	static constexpr int RESPONSE_CODE_SIZE_BYTES = 2;
	static constexpr int PAYLOAD_SIZE_SIZE_BYTES = 4;
	static constexpr int RESPONSE_HEADER_SIZE_BYTES = VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES + PAYLOAD_SIZE_SIZE_BYTES;

	// message requests
	static constexpr int MESSAGE_MAX_SIZE = 1000;

	static constexpr uint8_t SYM_KEY_REQ_MESSAGE = 1;
	static constexpr uint8_t SYM_KEY_SEND_MESSAGE = 2;
	static constexpr uint8_t TEXT_MESSAGE = 3;

	static constexpr int MESSAGE_CLIENTID_OFFSET = 0;
	static constexpr int MESSAGE_ID_OFFSET = MESSAGE_CLIENTID_OFFSET + CLIENTID_SIZE_BYTES;
	static constexpr int MESSAGE_TYPE_OFFSET = MESSAGE_ID_OFFSET + MESSAGE_ID_SIZE_BYTES;
	static constexpr int MESSAGE_CONTENT_SIZE_OFFSET = MESSAGE_TYPE_OFFSET + MESSAGE_TYPE_SIZE_BYTES;
	static constexpr int MESSAGE_CONTENT_OFFSET = MESSAGE_CONTENT_SIZE_OFFSET + MESSAGE_CONTENT_SIZE_SIZE_BYTES;




};