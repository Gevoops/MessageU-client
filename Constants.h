#pragma once

constexpr int VERSION = 1;

// byte sizes of data 
constexpr int VERSION_SIZE_BYTES = 1;

constexpr int CLIENTID_SIZE_BYTES = 16;
constexpr int USERNAME_SIZE_BYTES = 255;
constexpr int PUBLICKEY_SIZE_BYTES = 160;

// message
constexpr int MESSAGE_ID_SIZE_BYTES = 4;
constexpr int MESSAGE_TYPE_SIZE_BYTES = 1;
constexpr int MESSAGE_CONTENT_SIZE_SIZE_BYTES = 4;
constexpr int MESSAGE_HEADER_SIZE_BYTES = CLIENTID_SIZE_BYTES + 
										  MESSAGE_ID_SIZE_BYTES + 
	 									  MESSAGE_TYPE_SIZE_BYTES + 
	                                      MESSAGE_CONTENT_SIZE_SIZE_BYTES;









// request codes
constexpr int REGISTER_REQ = 600;
constexpr int USER_LIST_REQ = 601;
constexpr int PUB_KEY_REQ = 602;
constexpr int MESSAGE_REQ = 603;
constexpr int GET_MESSAGES_REQ = 604;


// response codes
constexpr int REG_SUCCESS_RESPONSE = 2100;
constexpr int USER_LIST_RESPONSE = 2101;
constexpr int PUBLIC_KEY_RESPONSE = 2102;
constexpr int MESSAGE_SENT_RESPONSE = 2103;
constexpr int WAITING_MESSAGES_RESPONSE = 2104;
constexpr int SERVER_ERROR_RESPONSE = 9000;





