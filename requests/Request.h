#pragma once

#include <string>
#include <cstring> 
#include <vector>
#include <cstdint> 
#include "../Communication.h"


class Request {
    protected:
        uint8_t m_clientID[16] = {0};
        static uint8_t version;
        uint16_t m_requestCode;
        uint32_t m_payloadSize;
        std::vector<uint8_t> m_payLoad;
    public:
        Request(uint16_t code, uint32_t dataSize);

        uint16_t getRequestCode() const;

        uint32_t getpayLoadSize() const;

        std::vector<uint8_t> serialize() const;

        void sendToServer(Communication &comm);
};

