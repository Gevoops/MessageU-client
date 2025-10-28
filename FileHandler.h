#pragma once
#include <string>

class FileHandler {
private:
	static std::string serverIp;
	static int serverPort;
public:
	FileHandler();
	const char* readServerIp();
	int readServerPort();

	bool isRegistered();
	void createMyInfo(std::string username, std::string uuid);

	static bool readClientID(uint8_t(&m_clientID)[16]);
};