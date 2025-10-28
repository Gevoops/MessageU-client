#include "ClientController.h"
#include <iostream>
#include <sstream>
#include "Constants.h"
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "requests/RequestSender.h"
#include "Contact.h"
#include <iomanip>



ClientController::ClientController(FileHandler& fileHandler, RequestSender& sender, ResponseReceiver& receiver) :
	m_fileHandler(fileHandler), m_sender(sender), m_receiver(receiver)
{
	
}

void ClientController::run()
{
	bool sentRequest;
	int choice = 1;
	while (choice) {
		printMenu();
		choice = readInput();
		sentRequest = handleChoice(choice);
		if (sentRequest) {
			handleResponse();
		}
	} 
}


void ClientController::printMenu()
{
	std::cout << "\n" <<
		"MessageU client at your service." << std::endl << std::endl <<
		"110) Register" << std::endl <<
		"120) Request for clients list" << std::endl <<
		"130) Request for public key" << std::endl <<
		"140) Request for waiting messages" << std::endl <<
		"150) Send a text message" << std::endl <<
		"151) Send a request for symmetric key" << std::endl <<
		"152) Send your symmetric key" << std::endl <<
		"0) Exit client" << std::endl << "?" << std::endl;
}

bool ClientController::handleChoice(int choice)
{
	switch (choice) {
		case REGISTER: {
			return registerClient();
			break;
		}
		case REQ_CLIENT_LIST: {
			return getClientList();
			break;
		}
		case REQ_PUBLIC_KEY: {
			return reqPublicKey();
			break;
		}
		case REQ_WAITING_MESSAGES: {
			return getWaitingMessages();
			break;
		}
		case SEND_TXT_MESSAGE: {
			return sendTextMessage();
			break;
		}
		case SEND_REQ_FOR_SYMM_KEY: {
			return reqSymmKey();
			break;
		}
		case SEND_SYMM_KEY: {
			return sendSymmKey();
			break;
		}
		case EXIT: {
			std::cout << "Exit was chosen, goodbye!" << std::endl;
			exit(0);
			break;
		}
		case INVALID_INPUT: {
			std::cout << "invalid menu choice\n";
			return false;
			break;
		}
		default:
			std::cout << "invalid menu choice\n";
			return false;
			break;
	}
	return true;
}

void ClientController::handleResponse()
{
	m_receiver.receiveResponse();
	char* buffer = m_receiver.getBuffer();
	short payloadSize = m_receiver.getPayloadSize();
	switch (m_receiver.getResponseCode()) {
		case REG_SUCCESS_RESPONSE: {
			regSuccess(buffer);
			break;
		}
		case USER_LIST_RESPONSE: {
			userListResponse(buffer, payloadSize);
			break;
		}
		case PUBLIC_KEY_RESPONSE: {
			publicKeyResponse(buffer);
			break;
		}
		case MESSAGE_SENT_RESPONSE: {
			messageReceivedByServer(buffer);
			break;

		}
		case WAITING_MESSAGES_RESPONSE: {
			waitingMessages(buffer, payloadSize);
			break;
		}
		case SERVER_ERROR_RESPONSE: {
			std::cout << "some error has occurred, server respone 9000" << std::endl;
			break;
		}
		default:
			std::cout << "Unknown server response\n";
			break;
	}
}



int ClientController::readInput()
{
	int input = INVALID_INPUT;
	std::string inputString;
	std::getline(std::cin, inputString);
	std::stringstream(inputString) >> input;
	return input;
}


bool ClientController::registerClient() {
	if (m_fileHandler.isRegistered()) {
		std::cout << "The file my.info already exists\n";
		return false;
	}
	std::cout << "Register was chosen" << std::endl;
	std::cout << "please choose a username. max length is 254 characters." << std::endl;
	std::getline(std::cin, m_registrationUsername);
	

	if (m_registrationUsername.size() > USERNAME_SIZE_BYTES - 1 || m_registrationUsername.size() == 0) {
		std::cout << "username not valid, please choose 1 - 254 chars";
		return false;
	}
	uint8_t publicKey[PUBLICKEY_SIZE_BYTES] = { "shahf" };
	m_sender.sendRegisterReq(m_registrationUsername, publicKey);
	return true;
}

bool ClientController::getClientList()
{
	std::cout << "120 ask for client list" << std::endl;
	if (!m_fileHandler.isRegistered()) {
		std::cout << "not registered yet. please use 110" << std::endl;
		return false;
	}
	m_sender.sendClientListReq();
	return true;
}

bool ClientController::reqPublicKey()
{
	std::cout << "130 ask for public key" << std::endl;
	std::cout << "Please enter the username of the user whose public key you want." << std::endl;

	std::string targetUsername;
	std::getline(std::cin, targetUsername);

	Contact* c = Contact::getContact(targetUsername);
	if (c == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}
	m_sender.sendPublicKeyReq(c);
	return true;
}

bool ClientController::getWaitingMessages()
{
	std::cout << "140 request for messages" << std::endl;
	if (Contact::isContactsEmpty()) {
		std::cout << "please use 120 first" << std::endl;
		return false;
	}
	m_sender.sendWaitingMessagesReq();
	return true;
}

bool ClientController::sendTextMessage()
{
	std::cout << "150 send a txt message" << std::endl;
	std::cout << "please enter target username" << std::endl;

	std::string targetUsername;
	std::getline(std::cin, targetUsername);

	Contact* c = Contact::getContact(targetUsername);
	if (c == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}
	std::cout << "please enter message content" << std::endl;

	std::string messageContent;
	std::getline(std::cin, messageContent);
	std::vector<uint8_t> contentVec(messageContent.begin(), messageContent.end());

	m_sender.sendMessageReq(c->getClientID(), TEXT_MESSAGE, static_cast<int>(messageContent.size()), contentVec);
	return true;
}

bool ClientController::reqSymmKey()
{
	std::cout << "151 request symmetric key!" << std::endl;
	std::string targetUsername;
	std::getline(std::cin, targetUsername);

	Contact* c = Contact::getContact(targetUsername);
	if (c == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}
	std::vector<uint8_t> messageContent;
	m_sender.sendMessageReq(c->getClientID(), SYM_KEY_REQ_MESSAGE, 0, messageContent);
	return true;
}

bool ClientController::sendSymmKey()
{
	std::cout << "152 send simetric key!" << std::endl;
	std::string targetUsername;
	std::getline(std::cin, targetUsername);

	Contact* c = Contact::getContact(targetUsername);
	if (c == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}

	uint8_t symmkey[50] = { 1 };
	std::vector<uint8_t> contentVec(std::begin(symmkey), std::end(symmkey));
	m_sender.sendMessageReq(c->getClientID(), SYM_KEY_SEND_MESSAGE, static_cast<int>(contentVec.size()), contentVec);

	return true;
}

void ClientController::regSuccess(char * buffer)
{
	uint8_t uuid[CLIENTID_SIZE_BYTES];
	std::ostringstream oss;
	for (int i = 0; i < CLIENTID_SIZE_BYTES; i++) {
		uuid[i] = buffer[i];
		oss << std::hex << std::setw(2) << std::setfill('0') << (int)uuid[i];
	}
	m_fileHandler.createMyInfo(m_registrationUsername, oss.str());
	std::cout << "registration completed successfully " << std::endl;
}

void ClientController::userListResponse(char* buffer, short payloadSize)
{
	std::cout << "2101 response! recieved contact list" << std::endl;

	Contact::contacts.clear();

	uint8_t clientID[CLIENTID_SIZE_BYTES];
	char username[USERNAME_SIZE_BYTES];

	for (int i = 0; i < payloadSize; i += USERNAME_SIZE_BYTES + CLIENTID_SIZE_BYTES) {
		std::memcpy(username, buffer + i, USERNAME_SIZE_BYTES);
		std::memcpy(clientID, buffer + i + USERNAME_SIZE_BYTES, CLIENTID_SIZE_BYTES);
		std::string s(username);
		Contact contact(s, clientID);
		Contact::contacts.push_back(contact);
	}

	for (const Contact& contact : Contact::contacts) {
		std::cout << contact.getUsername() << std::endl;
	}
	
}

void ClientController::publicKeyResponse(char* buffer)
{
	std::cout << "2102 respones! recieved target public key\n";
	uint8_t publicKey[PUBLICKEY_SIZE_BYTES];
	uint8_t clientID[CLIENTID_SIZE_BYTES];
	std::memcpy(clientID, buffer, CLIENTID_SIZE_BYTES);
	std::memcpy(publicKey, buffer + CLIENTID_SIZE_BYTES, PUBLICKEY_SIZE_BYTES);
	Contact* c = Contact::getContact(clientID);
	if (Contact::getContact(clientID) != nullptr) {
		c->setPublicKey(publicKey);
	}
}

void ClientController::messageReceivedByServer(char* buffer)
{
	uint32_t messageID;
	std::memcpy(&messageID, buffer + CLIENTID_SIZE_BYTES, MESSAGE_ID_SIZE_BYTES);
	messageID = ntohl(messageID);
	std::cout << "message sent and recieved by server! \n";
}

void ClientController::waitingMessages(char* buffer, short payloadSize)
{
	for (int i = 0; i < payloadSize;) {
		uint8_t senderID[CLIENTID_SIZE_BYTES];
		uint32_t messageID;
		uint8_t messageType;
		uint32_t messageContentSize = 0;
		std::string sender_username;

		std::memcpy(&senderID, buffer + MESSAGE_CLIENTID_OFFSET + i, CLIENTID_SIZE_BYTES);
		std::memcpy(&messageID, buffer + MESSAGE_ID_OFFSET + i, MESSAGE_ID_SIZE_BYTES);
		messageID = ntohl(messageID);
		std::memcpy(&messageType, buffer + MESSAGE_TYPE_OFFSET + i, MESSAGE_TYPE_SIZE_BYTES);
		std::memcpy(&messageContentSize, buffer + MESSAGE_CONTENT_SIZE_OFFSET + i, MESSAGE_CONTENT_SIZE_SIZE_BYTES);
		messageContentSize = ntohl(messageContentSize);

		Contact* c = Contact::getContact(senderID);

		if (c != nullptr) {
			sender_username = c->getUsername();
			std::cout << sender_username << "\n";
		}
		else {
			std::cout << "cant find sender\n";
			std::cout << "maybe call 120\n";
		}

		std::cout << "From: " << sender_username << "\n";
		std::cout << "Content:\n";
		switch (messageType) {
		case SYM_KEY_REQ_MESSAGE: {
			std::cout << "Request for symmetric key\n";
			break;
		}
		case SYM_KEY_SEND_MESSAGE: {
			std::cout << "symmetric key received\n";
			break;
		}
		case TEXT_MESSAGE: {
			std::string text;
			text.resize(messageContentSize);
			std::memcpy(&text[0], buffer + MESSAGE_CONTENT_OFFSET + i, messageContentSize);
			std::cout << text << "\n";
			break;
		}
		default:
			std::cout << "Unknown message type\n";
			break;
		}
		std::cout << "-----<EOM>-----\n";

		i += (MESSAGE_HEADER_SIZE_BYTES + messageContentSize);
	}
}





