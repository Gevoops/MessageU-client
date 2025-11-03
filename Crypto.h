#pragma once
#include <vector>

class Crypto {
public:
	Crypto();
	std::vector<uint8_t> genPublicPrivateKeyPair(std::vector<uint8_t>& publicKeyHolder, std::vector<uint8_t>& privateKeyHolder);
	std::vector<uint8_t> publicKeyEncrypt(const std::vector<uint8_t>& publicKey, const std::vector<uint8_t> & messageContent);
	std::vector<uint8_t> privateKeyDecrypt(const std::vector<uint8_t>& privbateKey, std::vector<uint8_t>& messageContent);
private:
};