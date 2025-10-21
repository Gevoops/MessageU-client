#pragma once
#include <cstdint> 
#include <vector>
#include <string>

class Contact {
private: 
	std::string username;
	uint8_t clientID[16];

public: 

	static std::vector<Contact> contacts;

	static Contact* getContact(std::string username);

	Contact(std::string username, const uint8_t* clientID);

	std::string getUsername() const;

	const uint8_t* getClientID() const;
		
};