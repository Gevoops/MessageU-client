#include <string>
#include <fstream>
#include <iostream>
#include "Communication.h"
#include "Utils.h"
#include "FileHandler.h"
#include <ws2tcpip.h>


Communication::Communication() {
	m_socket = initSocket();
}

SOCKET Communication::initSocket() {
	WSADATA wsaData;
	FileHandler fileHandler;
	sockaddr_in serverAddr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "Winsock initialization failed!" << std::endl;
		return INVALID_SOCKET;
	}
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Socket creation failed!" << WSAGetLastError() << std::endl;
		WSACleanup();
		return INVALID_SOCKET;
	}

	const char* serverIp = fileHandler.readServerIp();
	const int serverPort = fileHandler.readServerPort();

	Communication::initClientId();

	serverAddr.sin_port = htons(serverPort);
	serverAddr.sin_family = AF_INET;

	if (inet_pton(AF_INET, serverIp, &serverAddr.sin_addr) <= 0) {
		std::cerr << "invalid server adress" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}
	if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "Connection to server failed!" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return INVALID_SOCKET;
	}

	std::cout << "Connected to server!" << std::endl;
	return clientSocket;
}

SOCKET Communication::getSocket()
{
	return m_socket;
}

void Communication::initClientId() {
	std::string clientID_string;
	std::ifstream file("my.info");
	if (file) {
		std::getline(file, clientID_string);
		std::getline(file, clientID_string); // twice to get second row
		std::memcpy(Communication::clientID, Utils::stringToByteHexa(clientID_string), CLIENTID_SIZE_BYTES);
		file.close();
	}
}

bool Communication::isLittleEndian()
{
	uint16_t num = 1;
	return *(uint8_t*)&num == 1;
}

uint32_t Communication::swapEndian32(uint32_t num)
{
	return ((num & 0x000000FF) << 24) |
	       ((num & 0x0000FF00) << 8) |
		   ((num & 0x00FF0000) >> 8) |
		   ((num & 0xFF000000) >> 24);
}

uint16_t Communication::swapEndian16(uint16_t num)
{
	return ((num & 0x00FF) << 8) |
	       ((num & 0xFF00) >> 8);
}
