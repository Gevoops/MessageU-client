#pragma once

constexpr uint8_t VERSION = 1;

// byte sizes of data 
constexpr int VERSION_SIZE_BYTES = 1;

constexpr int CLIENTID_SIZE_BYTES = 16;
constexpr int USERNAME_SIZE_BYTES = 255;
constexpr int PUBLIC_KEY_SIZE_BYTES = 160;
constexpr int PRIVATE_KEY_SIZE_BYTES = 160;
constexpr int SYMM_KEY_SIZE_BYTES = 16;


// message
constexpr int MESSAGE_ID_SIZE_BYTES = 4;
constexpr int MESSAGE_TYPE_SIZE_BYTES = 1;
constexpr int MESSAGE_CONTENT_SIZE_SIZE_BYTES = 4;
constexpr int MESSAGE_HEADER_SIZE_BYTES = CLIENTID_SIZE_BYTES + 
										  MESSAGE_ID_SIZE_BYTES + 
	 									  MESSAGE_TYPE_SIZE_BYTES + 
	                                      MESSAGE_CONTENT_SIZE_SIZE_BYTES;





// request codes
constexpr uint16_t REGISTER_REQ = 600;
constexpr uint16_t USER_LIST_REQ = 601;
constexpr uint16_t PUB_KEY_REQ = 602;
constexpr uint16_t MESSAGE_REQ = 603;
constexpr uint16_t GET_MESSAGES_REQ = 604;





