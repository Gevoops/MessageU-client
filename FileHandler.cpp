#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <string>

std::string FileHandler::serverIp = "";
int FileHandler::serverPort = 0;

FileHandler::FileHandler() {
	
}

void FileHandler::createMyInfo(std::string username, std::string uuid) {
	std::ofstream file("my.info");
	if (!file) {
		std::cout << "error creating my.info file" << std::endl;
		exit(1);
	}
	file << username << std::endl;
	file << uuid;
	file.close();
}

bool FileHandler::isRegistered() {
	std::ifstream file("my.info");
	if (file) {
		return true;
	}
	return false;
}

const char* FileHandler::readServerIp() {
	std::ifstream file("server.info");
	if (!file) {
		std::cerr << "some problem with file server.info\n";
		return nullptr;
	}

	std::string serverInfo;
	std::getline(file, serverInfo);

	size_t index = serverInfo.find(':');
	serverIp = serverInfo.substr(0, index);
	
	return serverIp.c_str();
}

int FileHandler::readServerPort() {
	std::ifstream file("server.info");
	if (!file) {
		std::cerr << "some problem with file server.info\n";
	}

	std::string serverInfo;
	std::getline(file, serverInfo);

	size_t index = serverInfo.find(':');
	serverPort = std::stoi(serverInfo.substr(index + 1));

	return serverPort;
}
