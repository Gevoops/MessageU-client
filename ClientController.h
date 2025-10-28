#pragma once
#include "FileHandler.h"
#include "Communication.h"
#include "requests/RequestSender.h"
#include "ResponseReceiver.h"

class ClientController {
public:
	ClientController(FileHandler& fileHandler, RequestSender& sender, ResponseReceiver & receiver);
	void run();
	


private:
	
	void printMenu();
	bool handleChoice(int choice);
	int readInput();
	void handleResponse();
	bool registerClient();
	bool getClientList();
	bool reqPublicKey();
	bool getWaitingMessages();
	bool sendTextMessage();
	bool reqSymmKey();
	bool sendSymmKey();

	void regSuccess(char * buffer);
	void userListResponse(char* buffer, short payloadSize);
	void publicKeyResponse(char* buffer);
	void messageReceivedByServer(char* buffer);
	void waitingMessages(char* buffer, short payloadSize);


	std::string m_registrationUsername = "";
	FileHandler& m_fileHandler;
	RequestSender& m_sender;
	ResponseReceiver& m_receiver;
	

	static const int INVALID_INPUT = -1;
	static const int REGISTER = 110;
	static const int REQ_CLIENT_LIST = 120;
	static const int REQ_PUBLIC_KEY = 130;
	static const int REQ_WAITING_MESSAGES = 140;
	static const int SEND_TXT_MESSAGE = 150;
	static const int SEND_REQ_FOR_SYMM_KEY = 151;
	static const int SEND_SYMM_KEY = 152;
	static const int EXIT = 0;


	static const uint8_t SYM_KEY_REQ_MESSAGE = 1;
	static const uint8_t SYM_KEY_SEND_MESSAGE = 2;
	static const uint8_t TEXT_MESSAGE = 3;


	// response codes
	static const int REG_SUCCESS_RESPONSE = 2100;
	static const int USER_LIST_RESPONSE = 2101;
	static const int PUBLIC_KEY_RESPONSE = 2102;
	static const int MESSAGE_SENT_RESPONSE = 2103;
	static const int WAITING_MESSAGES_RESPONSE = 2104;
	static const int SERVER_ERROR_RESPONSE = 9000;


	static const int RESPONSE_CODE_SIZE_BYTES = 2;
	static const int PAYLOAD_SIZE_SIZE_BYTES = 4;
	static const int RESPONSE_HEADER_SIZE_BYTES = VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES + PAYLOAD_SIZE_SIZE_BYTES;
	static const int MESSAGE_CLIENTID_OFFSET = RESPONSE_HEADER_SIZE_BYTES;
	static const int MESSAGE_ID_OFFSET = RESPONSE_HEADER_SIZE_BYTES + CLIENTID_SIZE_BYTES;
	static const int MESSAGE_TYPE_OFFSET = MESSAGE_ID_OFFSET + MESSAGE_ID_SIZE_BYTES;
	static const int MESSAGE_CONTENT_SIZE_OFFSET = MESSAGE_TYPE_OFFSET + MESSAGE_TYPE_SIZE_BYTES;
	static const int MESSAGE_CONTENT_OFFSET = MESSAGE_CONTENT_SIZE_OFFSET + MESSAGE_CONTENT_SIZE_SIZE_BYTES;




};