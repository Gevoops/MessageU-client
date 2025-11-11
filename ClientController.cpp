#include "ClientController.h"
#include <iostream>
#include <sstream>
#include "Constants.h"
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Contact.h"
#include <iomanip>
#include "requests/AllRequests.h"
#include "encryption/Base64Wrapper.h"
#include "encryption/RSAWrapper.h"
#include "encryption/AESWrapper.h"


ClientController::ClientController(FileHandler& fileHandler, ResponseReceiver& receiver, Communication& comm) :
	m_fileHandler(fileHandler), m_receiver(receiver), m_comm(comm)
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
	uint32_t payloadSize = m_receiver.getPayloadSize();
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
			std::cout << "server responded with an error" << std::endl;
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
	if (inputString.size() > REQ_CODE_DIGITS) {
		return INVALID_INPUT;
	}
	for (char c : inputString) {
		if (!std::isdigit(c))
			return INVALID_INPUT;
	}
	return input;
}

std::string ClientController::inputUsername()
{
	std::string error_m("username not valid, please use 1 - 254 letters or numbers");
	std::string input;
	std::getline(std::cin, input);
	if (input.size() > USERNAME_SIZE_BYTES - 1 || input.size() == 0) {
		std::cout << error_m;
		return std::string();
	}

	for (char c : input) {
		if (!std::isalnum(static_cast<unsigned char>(c))) {
			std::cout << error_m;
			return std::string();
		}
	}
	return input;
}


bool ClientController::registerClient() {
	if (m_fileHandler.isRegistered()) {
		std::cout << "The file my.info already exists\n";
		return false;
	}

	std::cout << "Register was chosen" << std::endl;
	std::cout << "please choose a username. max length is 254 characters." << std::endl;
	m_registrationUsername = inputUsername();
	if (m_registrationUsername.empty()) {
		return false;
	}
	RSAPrivateWrapper rsapriv;
	m_privKey = rsapriv.getPrivateKey();
	std::string pubkey = rsapriv.getPublicKey();

	RegisterRequest request(m_registrationUsername, pubkey);
	request.sendToServer(m_comm);
	return true;
}

bool ClientController::getClientList()
{
	std::cout << "client list requested" << std::endl;
	if (!m_fileHandler.isRegistered()) {
		std::cout << "not registered yet. please register with 110" << std::endl;
		return false;
	}
	UserListRequest request{};
	request.sendToServer(m_comm);
	return true;
}

bool ClientController::reqPublicKey()
{
	std::cout << "130 ask for public key" << std::endl;
	std::cout << "Please enter the username of the user whose public key you want." << std::endl;

	std::string targetUsername = inputUsername();
	if (targetUsername.empty()) {
		return false;
	}

	Contact* contact = Contact::getContact(targetUsername);
	if (contact == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}
	PublicKeyRequest request{contact->getClientID()};
	request.sendToServer(m_comm);
	return true;
}

bool ClientController::getWaitingMessages()
{
	std::cout << "140 request for messages" << std::endl;
	if (Contact::isContactsEmpty()) {
		std::cout << "please use 120 first" << std::endl;
		return false;
	}
	GetMessagesRequest request{};
	request.sendToServer(m_comm);
	return true;
}

bool ClientController::sendTextMessage()
{
	std::cout << "150 send a txt message" << std::endl;
	std::cout << "please enter target username" << std::endl;

	std::string targetUsername = inputUsername();
	if (targetUsername.empty()) {
		return false;
	}

	Contact* contact = Contact::getContact(targetUsername);
	if (contact == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}
	if (!contact->isSymmKeySaved()) {
		std::cout << "please establish symmetric key pair before sending a message\n";
		return false;
	}
	std::cout << "please enter message content" << std::endl;

	std::string messageContent;
	std::getline(std::cin, messageContent);
	if (messageContent.size() > ClientController::MESSAGE_MAX_SIZE) {
		std::cout << "max message length is: " << ClientController::MESSAGE_MAX_SIZE 
			<< "the length of your message was: " << messageContent.size() << "\n";
		return false;
	}

	AESWrapper aes(contact->getSymmKey(), AESWrapper::DEFAULT_KEYLENGTH);
	std::string encryptedMessage = aes.encrypt(messageContent.c_str(), static_cast<unsigned int>(messageContent.length()));
	MessageRequest request(contact->getClientID(), TEXT_MESSAGE, static_cast<uint32_t>(encryptedMessage.size()), encryptedMessage);
	request.sendToServer(m_comm);
	return true;
}

bool ClientController::reqSymmKey()
{
	std::cout << "request for symmetric key!" << std::endl;
	std::cout << "please enter target username\n";
	std::string targetUsername = inputUsername();
	if (targetUsername.empty()) {
		return false;
	}

	Contact* contact = Contact::getContact(targetUsername);
	if (contact == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}
	// in the protocol it is said to encrypt this message. 
	// this message content is non existent and it is not clear to me what to encrypt or how it is possible
	// a forum question was asked, multiple times, and one answer was "if you think there is nothing to encrypt then dont". 
	// i didnt want it to seem like im ignoring protocol but i had to resolve this contradiction somehow.
	// so i decided to adhere to the protocol in terms of messaging order, but i will not encrypt this message type. 
	// i also think it is safe because the symmetric key is the actual secret, and i dont see why i should 
	// encrypt the message requesting for it.
	
	std::string messageContent;
	if (contact->getPublicKey().empty()) {
		std::cout << "before sending this request, please get the recipient public key\n";
		return false;
	}
	
	MessageRequest request(contact->getClientID(), SYM_KEY_REQ_MESSAGE, static_cast<uint32_t>(messageContent.size()), messageContent);
	request.sendToServer(m_comm);
	return true;
}

bool ClientController::sendSymmKey()
{
	std::cout << "send symmetric key!" << std::endl;
	std::string targetUsername = inputUsername();
	if (targetUsername.empty()) {
		return false;
	}

	Contact* contact = Contact::getContact(targetUsername);
	if (contact == nullptr) {
		std::cout << "contact not found. ask for contact list or try another\n";
		return false;
	}

	if (!contact->isPubKeySaved()) {
		std::cout << "Please get target contact public key before trying to send your symmetric key\n";
		return false;
	}

	unsigned char key[AESWrapper::DEFAULT_KEYLENGTH];
	AESWrapper aes(AESWrapper::GenerateKey(key, AESWrapper::DEFAULT_KEYLENGTH), AESWrapper::DEFAULT_KEYLENGTH);

	contact->setSymmKey(aes.getKey());

	RSAPublicWrapper rsapub(contact->getPublicKey());
	std::string encryptedSymmKey = rsapub.encrypt((const char *)aes.getKey(), AESWrapper::DEFAULT_KEYLENGTH);
	
	
	MessageRequest request(contact->getClientID(), SYM_KEY_SEND_MESSAGE, static_cast<uint32_t>(encryptedSymmKey.size()), encryptedSymmKey);
	request.sendToServer(m_comm);
	return true;
}

void ClientController::regSuccess(char * buffer)
{
	uint8_t uuid[CLIENTID_SIZE_BYTES];
	std::ostringstream oss;
	oss << std::hex << std::setfill('0');
	for (int i = 0; i < CLIENTID_SIZE_BYTES; i++) {
		uuid[i] = buffer[i];
		oss << std::hex << std::setw(2) << std::setfill('0') << (int)uuid[i];
	}
	m_fileHandler.createMyInfo(m_registrationUsername, oss.str(), m_privKey);

	if (m_privKey != m_fileHandler.readPrivateKey()) {
		std::cout << "doesnt work brrother\n";
		std::cout << m_privKey.length() << " ";
		std::cout << m_fileHandler.readPrivateKey().length() << "\n";

	}
	std::cout << "registration completed successfully " << std::endl;
}

void ClientController::userListResponse(char* buffer, uint32_t payloadSize)
{
	std::cout << "recieved client list" << std::endl;

	std::vector<Contact> contactList;

	uint8_t clientID[CLIENTID_SIZE_BYTES];

	for (uint32_t i = 0; i < payloadSize; i += USERNAME_SIZE_BYTES + CLIENTID_SIZE_BYTES) {
		std::string username(buffer + i, buffer + i + USERNAME_SIZE_BYTES);
		std::memcpy(clientID, buffer + i + USERNAME_SIZE_BYTES, CLIENTID_SIZE_BYTES);
		username.erase(std::find(username.begin(), username.end(), '\0'), username.end());
		Contact contact(username, clientID);
		contactList.push_back(contact);
	}

	for (const Contact& contact : contactList) {
		std::cout << contact.getUsername() << std::endl;
	}

	Contact::updateContacts(contactList);
}

void ClientController::publicKeyResponse(char* buffer)
{
	std::cout << "recieved public key\n";
	uint8_t clientID[CLIENTID_SIZE_BYTES];
	std::memcpy(clientID, buffer, CLIENTID_SIZE_BYTES);
	std::string publicKey(buffer + CLIENTID_SIZE_BYTES, buffer + CLIENTID_SIZE_BYTES + PUBLIC_KEY_SIZE_BYTES);
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

void ClientController::waitingMessages(char* buffer, uint32_t payloadSize)
{
	uint32_t messageContentSize = 0;
	for (uint32_t i = 0; i < payloadSize; i += (MESSAGE_HEADER_SIZE_BYTES + messageContentSize)) {
		uint8_t senderID[CLIENTID_SIZE_BYTES];
		uint32_t messageID;
		uint8_t messageType;
		
		std::string sender_username;

		std::memcpy(&senderID, buffer + MESSAGE_CLIENTID_OFFSET + i, CLIENTID_SIZE_BYTES);
		std::memcpy(&messageID, buffer + MESSAGE_ID_OFFSET + i, MESSAGE_ID_SIZE_BYTES);
		std::memcpy(&messageType, buffer + MESSAGE_TYPE_OFFSET + i, MESSAGE_TYPE_SIZE_BYTES);
		std::memcpy(&messageContentSize, buffer + MESSAGE_CONTENT_SIZE_OFFSET + i, MESSAGE_CONTENT_SIZE_SIZE_BYTES);

		Contact* contact = Contact::getContact(senderID);

		if (contact != nullptr) {
			sender_username = contact->getUsername();
		}
		else {
			std::cout << "sender client for this messsage wasnt found. try requesting for client list\n";
			continue;
		}

		std::cout << "From: " << sender_username << "\n";
		std::cout << "Content:\n";
		switch (messageType) {
		case SYM_KEY_REQ_MESSAGE: {
			std::cout << "Request for symmetric key\n";
			break;
		}
		case SYM_KEY_SEND_MESSAGE: {
			std::string encryptedSymmKey(buffer + MESSAGE_CONTENT_OFFSET + i, buffer + MESSAGE_CONTENT_OFFSET + i + messageContentSize);
			RSAPrivateWrapper decryptor(m_fileHandler.readPrivateKey());
			try {
				std::string decryptedSymmKey = decryptor.decrypt(encryptedSymmKey);
				contact->setSymmKey(reinterpret_cast<const unsigned char*>(decryptedSymmKey.data()));
				std::cout << "symmetric key received\n";
			}
			catch (const std::exception& e) {
				std::cerr << e.what() << "\n";
				std::cerr << "can’t decrypt symmetric key message \n";
			}
			
			break;
		}
		case TEXT_MESSAGE: {
			std::string EncryptedMessage(buffer + MESSAGE_CONTENT_OFFSET + i, buffer + MESSAGE_CONTENT_OFFSET + i + messageContentSize);

			try {
				if (!contact->isSymmKeySaved()) {
					throw std::runtime_error("no symmetric key saved for this sender: " + contact->getUsername());
				}
				AESWrapper decryptor(contact->getSymmKey(), AESWrapper::DEFAULT_KEYLENGTH);
				std::string decryptedMessage = decryptor.decrypt(EncryptedMessage.c_str(), static_cast<unsigned int>(EncryptedMessage.length()));
				std::cout << decryptedMessage << "\n";
			}
			catch (const std::exception& e){
				std::cerr << e.what() << "\n";
				std::cerr << "can’t decrypt message \n";
			}
			
			break;
		}
		default:
			std::cout << "Unknown message type\n";
			break;
		}
		std::cout << "-----<EOM>-----\n";

		
	}
}





