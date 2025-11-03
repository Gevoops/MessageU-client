#pragma once
#include <cstdint> 
#include <vector>
#include <string>
#include "constants.h"

class Contact {
private: 
	std::string username;
	uint8_t clientID[CLIENTID_SIZE_BYTES];
	std::vector<uint8_t> m_publicKey;
	std::vector<uint8_t> m_symmKey;

public: 
	static std::vector<Contact> contacts;

	static Contact* getContact(std::string username);

	static Contact* getContact(const uint8_t * id);

	static const bool isContactsEmpty();

	void setPublicKey(std::vector<uint8_t> newPublicKey);
	const std::vector<uint8_t>& getPublicKey() const;

	void setSymmKey(std::vector<uint8_t> newSymmKey);
	const std::vector<uint8_t>& getSymmKey() const;


	Contact(std::string username, const uint8_t* clientID);

	std::string getUsername() const;

	const uint8_t* getClientID() const;
	
	
		
};