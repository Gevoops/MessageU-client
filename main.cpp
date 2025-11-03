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


#pragma comment(lib, "ws2_32.lib")


int main() {
	FileHandler fileHandler;
	Communication comm; // init socket
	RequestSender sender(comm);
	ResponseReceiver receiver(comm);
	Crypto crypto;
	ClientController controller(fileHandler, sender, receiver, crypto);

	controller.run();

	return 0;
}