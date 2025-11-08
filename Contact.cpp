#include "Contact.h"
#include <iostream>
#include "Constants.h"
#include <iomanip>
#include <cstdint>
#include <algorithm>

std::vector<Contact> Contact::contacts;

Contact::Contact(std::string name, const uint8_t * id) {
	m_username = std::move(name);
	std::memcpy(m_clientID, id, CLIENTID_SIZE_BYTES);
	std::fill(std::begin(m_symmKey), std::end(m_symmKey), 0);
	m_symmKeySaved = false;
	m_publicKeySaved = false;
}

std::string Contact::getUsername() const{
	return m_username;
}

const uint8_t* Contact::getClientID() const{
	return m_clientID;
}

const bool Contact::isSymmKeySaved() const
{
	return m_symmKeySaved;
}

const bool Contact::isPubKeySaved() const
{
	return m_publicKeySaved;
}

const bool Contact::isContactsEmpty() 
{
	return contacts.size() == 0;
}

void Contact::setPublicKey(std::string newPublicKey) {
	m_publicKey = std::move(newPublicKey);
	m_publicKeySaved = true;
}

const std::string& Contact::getPublicKey() const
{
	return m_publicKey;
}

void Contact::setSymmKey(const unsigned char * newSymmKey)
{
	memcpy(m_symmKey, newSymmKey, SYMM_KEY_SIZE_BYTES);
	m_symmKeySaved = true;
}

const unsigned char * Contact::getSymmKey() const
{
	return m_symmKey;
}

void Contact::updateContacts(std::vector<Contact> &contactList)
{
	std::vector<Contact> newContacts;
	for (const auto& c1 : contactList) {
		bool found = std::any_of(contacts.begin(), contacts.end(),
			[&](const Contact& c2) {
				return c1.getUsername() == c2.getUsername();
			});
		if (!found)
			newContacts.push_back(c1);
	}
	contacts.insert(contacts.end(), newContacts.begin(), newContacts.end());
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
		if (std::memcmp(id, contact.m_clientID, CLIENTID_SIZE_BYTES) == 0) {
			return &contact;
		}
	}
	return nullptr;
}

