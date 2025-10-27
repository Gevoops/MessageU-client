#pragma once
#include <cstdint> 
#include <vector>
#include <string>
#include "constants.h"

class Contact {
private: 
	std::string username;
	uint8_t clientID[CLIENTID_SIZE_BYTES];
	uint8_t publicKey[PUBLICKEY_SIZE_BYTES] = {0};

public: 
	static std::vector<Contact> contacts;

	static Contact* getContact(std::string username);

	static Contact* getContact(const uint8_t * id);

	static const bool isContactsEmpty();

	void setPublicKey(const uint8_t* newPublicKey);

	Contact(std::string username, const uint8_t* clientID);

	std::string getUsername() const;

	const uint8_t* getClientID() const;
	
	
		
};