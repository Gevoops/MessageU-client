#pragma once
#include <string>
#include <vector>

class FileHandler {
private:
	static std::string serverIp;
	static int serverPort;
public:
	FileHandler();
	const char* readServerIp();
	int readServerPort();
	std::string readPrivateKey();

	bool isRegistered();
	void createMyInfo(std::string username, std::string uuid, const std::string privKey);
	static bool readClientID(uint8_t(&m_clientID)[16]);
};