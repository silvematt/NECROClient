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

public:
	// GCM IV
	struct IV
	{
		uint32_t prefix;	// 4 bytes random prefix
		uint64_t counter;	// 8 bytes counter

		bool randomized = false;

		IV()
		{
			RandomizePrefix();
			ResetCounter();
			randomized = false;
		}

		void RandomizePrefix()
		{
			RAND_bytes(reinterpret_cast<unsigned char*>(&prefix), sizeof(prefix));
			randomized = true;
		}

		void ResetCounter()
		{
			counter = 0;
		}

		void IncrementCounter()
		{
			counter++;
		}

		void ToByteArray(std::array<uint8_t, GCM_IV_SIZE>& out)
		{
			// Copy prefix (4 bytes)
			std::memcpy(out.data(), &prefix, sizeof(prefix));

			// Copy counter (8 bytes) after prefix
			std::memcpy(out.data() + sizeof(prefix), &counter, sizeof(counter));
		}
	};

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
