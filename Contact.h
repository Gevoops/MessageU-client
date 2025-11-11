#pragma once
#include <cstdint> 
#include <vector>
#include <string>
#include "encryption/AESWrapper.h"
#include "Constants.h"

class Contact {
private: 
	std::string m_username;
	uint8_t m_clientID[CLIENTID_SIZE_BYTES];
	std::string m_publicKey;
	unsigned char m_symmKey[AESWrapper::DEFAULT_KEYLENGTH];
	bool m_symmKeySaved;
	bool m_publicKeySaved;

public: 
	static std::vector<Contact> contacts;

	static void updateContacts(std::vector<Contact> &contactList);

	static Contact* getContact(std::string username);

	static Contact* getContact(const uint8_t * id);

	static const bool isContactsEmpty();

	void setPublicKey(std::string newPublicKey);

	const std::string& getPublicKey() const;

	void setSymmKey(const unsigned char * m_symmKey);

	const unsigned char * getSymmKey() const;


	Contact(std::string username, const uint8_t* clientID);

	std::string getUsername() const;

	const uint8_t* getClientID() const;

	const bool isSymmKeySaved() const;

	const bool isPubKeySaved() const;
	
};