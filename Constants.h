#pragma once

constexpr int VERSION = 1;

// byte sizes of data 
constexpr int VERSION_SIZE_BYTES = 1;
constexpr int RESPONSE_CODE_SIZE_BYTES = 2;
constexpr int PAYLOAD_SIZE_SIZE_BYTES = 4;
constexpr int RESPONSE_HEADER_SIZE_BYTES = VERSION_SIZE_BYTES + RESPONSE_CODE_SIZE_BYTES + PAYLOAD_SIZE_SIZE_BYTES;
constexpr int CLIENTID_SIZE_BYTES = 16;
constexpr int USERNAME_SIZE_BYTES = 255;
constexpr int PUBLICKEY_SIZE_BYTES = 160;
constexpr int MESSAGE_TYPE_SIZE_BYTES = 1;
constexpr int MESSAGE_CONTENT_SIZE_SIZE_BYTES = 4;



// request codes
constexpr int REGISTER_REQ_CODE = 600;
constexpr int USER_LIST_REQ_CODE = 601;
constexpr int PUB_KEY_REQ_CODE = 602;
constexpr int MESSAGE_REQ_CODE = 603;

// response codes
constexpr int REG_SUCCESS_CODE = 2100;
constexpr int USER_LIST_CODE = 2101;
constexpr int PUBLIC_KEY_CODE = 2102;
constexpr int SERVER_ERROR_CODE = 9000;




