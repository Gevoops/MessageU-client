#include "Contact.h"
#include <iostream>
#include "Constants.h"

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

Contact* Contact::getContact(std::string username) {
	for (Contact& contact : contacts) {
		std::cout << "iterate contact username is: " << contact.getUsername() << "\n";
		std::cout << "target username is " << username << "\n";
		if (contact.getUsername() == username) {
			std::cout << "found match\n";
			return &contact;
		}
	}
	std::cout << "not found bro " << username << "\n";
	return nullptr;
}

