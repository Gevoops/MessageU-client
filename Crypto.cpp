#include "Crypto.h"
#include "Constants.h"

Crypto::Crypto()
{
}

std::vector<uint8_t> Crypto::genPublicPrivateKeyPair(std::vector<uint8_t>& publicKeyHolder, std::vector<uint8_t>& privateKeyHolder)
{
	return std::vector<uint8_t>(PUBLIC_KEY_SIZE_BYTES,1);
}

std::vector<uint8_t> Crypto::publicKeyEncrypt(const std::vector<uint8_t>& publicKey, const std::vector<uint8_t>& messageContent)
{
	std::vector<uint8_t> res;
	if (publicKey.size() == 0) {
		return res;
	}
	res = { 1 };
	return res;
}

std::vector<uint8_t> Crypto::privateKeyDecrypt(const std::vector<uint8_t>& privbateKey, std::vector<uint8_t>& messageContent)
{
	return std::vector<uint8_t>();
}

