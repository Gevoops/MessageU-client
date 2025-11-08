#include <iostream>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <string>
#include <cstring>
#include <fstream>
#include "Constants.h"
#include "Contact.h"
#include "Utils.h"
#include "Communication.h"
#include "FileHandler.h"
#include "requests/AllRequests.h"
#include "ClientController.h"
#include "ResponseReceiver.h"
#include "encryption/Crypto.h"
#include "encryption/AESWrapper.h"
#include "encryption/Base64Wrapper.h"
#include "encryption/RSAWrapper.h"




int main() {
	FileHandler fileHandler;
	Communication comm; // init socket
	if (comm.getSocket() == INVALID_SOCKET) {
		std::cout << "connection to server couldn't be formed, please try again later\n";
		return 1;
	}
	ResponseReceiver receiver(comm);
	Crypto crypto;
	ClientController controller(fileHandler, receiver, crypto, comm);

	controller.run();

	return 0;
}




