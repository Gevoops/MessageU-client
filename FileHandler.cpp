#include "FileHandler.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Constants.h"
#include "encryption/Base64Wrapper.h"

std::string FileHandler::serverIp = "";
int FileHandler::serverPort = 0;

FileHandler::FileHandler() {
	
}

void FileHandler::createMyInfo(std::string username, std::string uuid, const std::string privKey) {
	std::ofstream file("my.info");
	if (!file) {
		std::cout << "error creating my.info file" << std::endl;
		exit(1);
	}
	file << username << std::endl;
	file << uuid << std::endl;
	file << Base64Wrapper::encode(privKey) << std::endl;
	file.close();
}

bool FileHandler::readClientID(uint8_t(&m_clientID)[16])
{
	std::ifstream file("my.info");
	if (!file) return false;
	
	std::string cid;

	std::getline(file, cid);

	if (!std::getline(file, cid)) return false;
	if (cid.size() != 32) return false;

	for (size_t i = 0; i < 16; ++i) {
		std::string byteString = cid.substr(i * 2, 2);
		m_clientID[i] = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
	}
	return true;
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

std::string FileHandler::readPrivateKey()
{
	std::ifstream file("my.info");
	if (!file) return std::string();

	std::string privKey;

	std::getline(file, privKey);
	std::getline(file, privKey);
	std::getline(file, privKey);

	return Base64Wrapper::decode(privKey);
}
