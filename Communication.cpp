#include <string>
#include <fstream>
#include <iostream>
#include "Communication.h"
#include "Utils.h"
#include "FileHandler.h"
#include <ws2tcpip.h>


Communication::Communication() {
	m_socket = initSocket();
	initClientId();
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

SOCKET Communication::getSocket() const
{
	return m_socket;
}

void Communication::initClientId() {
	std::string clientID_string;
	std::ifstream file("my.info");
	if (file) {
		std::cout << "reading client id from file" << std::endl;
		std::getline(file, clientID_string);
		std::getline(file, clientID_string); // twice to get second row
		std::cout << "file content is: " + clientID_string << std::endl;
		std::memcpy(Communication::clientID, Utils::stringToByteHexa(clientID_string), CLIENTID_SIZE_BYTES);
		file.close();
	}
}