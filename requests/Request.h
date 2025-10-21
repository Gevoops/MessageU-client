#pragma once

#include <string>
#include <cstring> 
#include <vector>
#include <cstdint> 


class Request {
    protected:
        uint8_t clientID[16];
        static uint8_t version;
        uint16_t requestCode;
        uint32_t payloadSize;
        std::vector<uint8_t> payLoad;

        

    public:
        Request(uint16_t code, uint32_t dataSize);

        const uint8_t* getClientID() const;

        uint16_t getRequestCode() const;

        uint32_t getpayLoadSize() const;

        std::vector<uint8_t> serialize() const;

};

