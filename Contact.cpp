#include "Contact.h"
#include <iostream>
#include "Constants.h"
#include <iomanip>
#include <cstdint>

std::vector<Contact> Contact::contacts;

Contact::Contact(std::string name, const uint8_t * id) {
	username = std::move(name);
	std::memcpy(clientID, id, CLIENTID_SIZE_BYTES);
}

std::string Contact::getUsername() const{
	return username;
}

const uint8_t* Contact::getClientID() const{
	return clientID;
}

const bool Contact::isContactsEmpty() 
{
	return contacts.size() == 0;
}

void Contact::setPublicKey(std::vector<uint8_t> newPublicKey) {
	m_publicKey = std::move(newPublicKey);
}

const std::vector<uint8_t>& Contact::getPublicKey() const
{
	return m_publicKey;
}

void Contact::setSymmKey(std::vector<uint8_t> newSymmKey)
{
	m_symmKey = std::move(newSymmKey);
}

const std::vector<uint8_t>& Contact::getSymmKey() const
{
	return m_symmKey;
}

Contact* Contact::getContact(std::string username) {
	for (Contact& contact : contacts) {
		if (contact.getUsername() == username) {
			return &contact;
		}
	}
	return nullptr;
}


Contact* Contact::getContact(const uint8_t * id) {
	for (Contact& contact : contacts) {
		if (std::memcmp(id, contact.clientID, CLIENTID_SIZE_BYTES) == 0) {
			return &contact;
		}
	}
	return nullptr;
}

