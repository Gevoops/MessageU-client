#include <iostream>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "Constants.h"
#include "Contact.h"
#include "Utils.h"
#include "Communication.h"
#include "FileHandler.h"
#include "requests/AllRequests.h"


#pragma comment(lib, "ws2_32.lib")


int main() {
	uint8_t publicKey[PUBLICKEY_SIZE_BYTES] = {"shahf"};
	FileHandler fileHandler;

	SOCKET clientSocket = Communication::initSocket();
	if (clientSocket == INVALID_SOCKET) {
		return 1;
	}


	int input = 1;
	while (input) {
		// user menu 
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

		std::string inputString;
		std::getline(std::cin, inputString);
		input = 0;
		std::stringstream(inputString) >> input;  
		if (!input) input = 5;
		
		std::string username;
		switch (input) {
			case REGISTER: {
				if (fileHandler.isRegistered()) { 
					std::cout << "my.info already exists\n";
					continue; }
				std::cout << "Register was chosen" << std::endl;
				std::cout << "please choose a username. max length is 254 characters." << std::endl;

				std::getline(std::cin, username);
	

				if (username.size() > USERNAME_SIZE_BYTES - 1 || username.size() == 0) {
					std::cout << "username not valid, please choose 1 - 254 chars";
					continue;
				}
				std::vector<uint8_t> dataVec;
				RegisterRequest request{ username, publicKey };

				dataVec = request.serialize();

				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
				break;
			}
			case 120: {
				std::cout << "120 ask for client list" << std::endl;
				if (!fileHandler.isRegistered()) {
					std::cout << "not registered yet. please use 110" << std::endl;
					continue;
				}

				std::vector<uint8_t> dataVec;
				UserListRequest request{};

				dataVec = request.serialize();

				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
				std::cout << "done 120" << std::endl;
				break;
			}
			case 130: {
				std::cout << "130 ask for public key" << std::endl;
				std::cout << "Please enter the username of the user whose public key you want." << std::endl;

				std::string targetUsername;
				std::getline(std::cin, targetUsername);

				Contact* c = Contact::getContact(targetUsername);
				if (c == nullptr) {
					std::cout << "contact not found. ask for contact list or try another\n";
					continue;
				}

				std::vector<uint8_t> dataVec;
				PublicKeyRequest request{ c->getClientID() };
				dataVec = request.serialize();
				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
				break;
			}
			case 140: {
				if (Contact::isContactsEmpty()) {
					std::cout << "please use 120 first" << std::endl;
					continue;
				}
				std::cout << "140 request for messages" << std::endl;
				GetMessagesRequest request{};
				std::vector<uint8_t> dataVec;
				dataVec = request.serialize();
				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
				break;
			}
			case 150: {
				std::cout << "150 send a txt message" << std::endl;
				std::cout << "please enter target username" << std::endl;

				std::string targetUsername;
				std::getline(std::cin, targetUsername);

				Contact* c = Contact::getContact(targetUsername);
				if (c == nullptr) {
					std::cout << "contact not found. ask for contact list or try another\n";
					continue;
				}
				std::cout << "please enter message content" << std::endl;

				std::string messageContent;
				std::getline(std::cin, messageContent);
			
				std::vector<uint8_t> dataVec(messageContent.begin(), messageContent.end());
				MessageRequest request{ c->getClientID(),3,static_cast<int>(messageContent.size()), dataVec };
			
				dataVec = request.serialize();
				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
				break;
			}
			case 151: {
				std::cout << "151 request symmetric key!" << std::endl;
				std::string targetUsername;
				std::getline(std::cin, targetUsername);

				Contact* c = Contact::getContact(targetUsername);
				if (c == nullptr) {
					std::cout << "contact not found. ask for contact list or try another\n";
					continue;
				}
				std::vector<uint8_t> dataVec;

				MessageRequest request{ c->getClientID(),SYM_KEY_REQ_MESSAGE,0, dataVec};

				dataVec = request.serialize();
				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);

				break;
			}
			case 152: {
				std::cout << "152 send simetric key!" << std::endl;
				std::string targetUsername;
				std::getline(std::cin, targetUsername);

				Contact* c = Contact::getContact(targetUsername);
				if (c == nullptr) {
					std::cout << "contact not found. ask for contact list or try another\n";
					continue;
				}

				uint8_t symmkey[50] = { 1 };
				std::vector<uint8_t> dataVec(std::begin(symmkey), std::end(symmkey));
				MessageRequest request{ c->getClientID(),SYM_KEY_SEND_MESSAGE ,static_cast<int>(dataVec.size()), dataVec };

				dataVec = request.serialize();
				send(clientSocket, reinterpret_cast<const char*>(dataVec.data()), static_cast<int>(dataVec.size()), 0);
				break;
			}
			case 0: {
				std::cout << "Exit was chosen, goodbye!" << std::endl;
				closesocket(clientSocket);
				return 0;
				break;
			}

			default: {
				std::cout << "thats not an option sweetie\n";
				std::cout << "try one of the menu options!\n";
				continue;
				break;
			}
		}
					



		char buffer[1024 * 50];
	
		int bytesReceived =  recv(clientSocket, buffer, RESPONSE_HEADER_SIZE_BYTES, 0);

		short serverResponseCode;
		int payloadSize;
		

		std::memcpy(&serverResponseCode, &buffer[VERSION_SIZE_BYTES], RESPONSE_CODE_SIZE_BYTES);
		std::memcpy(&payloadSize, &buffer[VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES], PAYLOAD_SIZE_SIZE_BYTES);

		std::cout << "got response" << std::endl;
		std::cout << "payloadSize " << payloadSize << "\n";

		for (int i = 0; i < payloadSize; i += 1024) {
			bytesReceived += recv(clientSocket, &buffer[RESPONSE_HEADER_SIZE_BYTES + i], 1024, 0);	
		}
		std::cout << "response code: " << serverResponseCode << "\n";
		
		switch (serverResponseCode) {
			case REG_SUCCESS_RESPONSE: {
				uint8_t uuid[CLIENTID_SIZE_BYTES];
				int start_index = RESPONSE_HEADER_SIZE_BYTES;
				std::ostringstream oss;
				for (int i = 0; i < CLIENTID_SIZE_BYTES; i++) {
					uuid[i] = buffer[i + start_index];
					oss << std::hex << std::setw(2) << std::setfill('0') << (int)uuid[i];
				}
				std::cout << oss.str();
				std::cout << std::endl;
				fileHandler.createMyInfo(username, oss.str());
				std::cout << "registration completed successfully " << std::endl;
				Communication::initClientId();
				
				break;

			}
			case USER_LIST_RESPONSE: {
				std::cout << "2101 response! recieved contact list" << std::endl;

				Contact::contacts.clear();

				uint8_t clientID[CLIENTID_SIZE_BYTES];
				char username[USERNAME_SIZE_BYTES]; 

				for (int i = 0; i < payloadSize; i +=  USERNAME_SIZE_BYTES + CLIENTID_SIZE_BYTES) {
					std::memcpy(username, buffer + RESPONSE_HEADER_SIZE_BYTES + i, USERNAME_SIZE_BYTES);
					std::memcpy(clientID, buffer + RESPONSE_HEADER_SIZE_BYTES + i + USERNAME_SIZE_BYTES, CLIENTID_SIZE_BYTES);
					std::string s(username);
					Contact contact(s, clientID);
					Contact::contacts.push_back(contact);
				}

				for (const Contact& contact : Contact::contacts) {
					std::cout << contact.getUsername() << std::endl;
				}
				break;

			}
			case PUBLIC_KEY_RESPONSE: {
				std::cout << "2102 respones! recieved target public key\n";
				uint8_t publicKey[PUBLICKEY_SIZE_BYTES];
				uint8_t clientID[CLIENTID_SIZE_BYTES];
				std::memcpy(clientID, buffer + RESPONSE_HEADER_SIZE_BYTES, CLIENTID_SIZE_BYTES);
				std::memcpy(publicKey, buffer + RESPONSE_HEADER_SIZE_BYTES + CLIENTID_SIZE_BYTES, PUBLICKEY_SIZE_BYTES);
				Contact* c = Contact::getContact(clientID);
				if (Contact::getContact(clientID) != nullptr) {
					c->setPublicKey(publicKey);
				}
				break;
			}
			case MESSAGE_SENT_RESPONSE: {
				uint32_t messageID;
				std::memcpy(&messageID, buffer + RESPONSE_HEADER_SIZE_BYTES + 16, 4);
				messageID = ntohl(messageID);
				std::cout << "message id is: " << messageID << "\n";
				std::cout << "message sent and recieved by server! \n";
				break;

			}
			case WAITING_MESSAGES_RESPONSE: {
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
						std::cout <<sender_username <<"\n";
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

	return 0;
}