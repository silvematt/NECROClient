#ifndef NECRO_AES_H
#define NECRO_AES_H

#include <array>
#include <stdexcept>

#include <openssl/evp.h>
#include <openssl/rand.h>

#define AES_128_KEY_SIZE 16	// 128-bit key
#define GCM_IV_SIZE 12
#define GCM_TAG_SIZE 16

class NECROAES
{
private:
	EVP_CIPHER_CTX* ctx;


public:
	static std::array<uint8_t, AES_128_KEY_SIZE> GenerateSessionKey()
	{
		std::array<uint8_t, AES_128_KEY_SIZE> k{};
		if (RAND_bytes(k.data(), AES_128_KEY_SIZE) != 1)
		{
			throw std::runtime_error("Failed to generate AES session key");
		}

		return k;
	}
};

#endif
