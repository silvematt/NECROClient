#ifndef NECRO_OPEN_SSL_MANAGER
#define NECRO_OPEN_SSL_MANAGER

#include <openssl/ssl.h>

#include "ConsoleLogger.h"
#include "FileLogger.h"

#ifdef _WIN32
#include "WinSock2.h"
#include <WS2tcpip.h>
typedef SOCKET sock_t;
#else
typedef int sock_t;
#endif

/*
Order is:
1) Create Socket and connect
2) Create CTX, BIO and SSLObject and link them to the Socket we just created
3) Setup the server's hostname for the handshake 
4) Set the server's hostname we'd expect to see in the cert
5) Perform the handshake
6) Exchange data
*/

class OpenSSLManager
{
private:
	static SSL_CTX* ctx;

public:
	static int Init()
	{
		// Create Context
		ctx = SSL_CTX_new(TLS_client_method());

		// Set verify of the certs
		SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

		// Use the default trusted certificate store
		if (!SSL_CTX_set_default_verify_paths(ctx)) 
		{
			LOG_ERROR("OpenSSLManager: Could not set default verify paths.");
			return 1;
		}

		// Restrict to TLS v1.3
		if (!SSL_CTX_set_min_proto_version(ctx, TLS1_3_VERSION)) 
		{
			LOG_ERROR("OpenSSLManager: failed to set min protocol version.");
			return 2;
		}

		LOG_OK("OpenSSLManager: Initialization Completed!");
		return 0;
	}

	static BIO* CreateBioAndWrapSocket(sock_t s)
	{
		BIO* b = BIO_new(BIO_s_socket());

		if (b == NULL)
		{
			LOG_ERROR("OpenSSLManager: failed to create a BIO socket object.");
			return nullptr;
		}

		// Wrap the socket | Socket will be closed when BIO is freed with the BIO_CLOSE option
		BIO_set_fd(b, s, BIO_CLOSE);

		return b;
	}

	static SSL* CreateSSLObject(BIO* setBio = nullptr)
	{
		SSL* s = SSL_new(ctx);

		if (s == NULL)
		{
			LOG_ERROR("OpenSSLManager: failed to create a SSL object.");
			return nullptr;
		}

		if (setBio != nullptr)
		{
			SSL_set_bio(s, setBio, setBio);
		}

		return s;
	}

	static void SetSNIHostname(SSL* s, const char* hostname)
	{
		if (!SSL_set_tlsext_host_name(s, hostname)) 
		{
			LOG_ERROR("OpenSSLManager: failed to SetSNIHostname.");
		}
	}

	static void SetCertVerificationHostname(SSL* s, const char* hostname)
	{
		if (!SSL_set1_host(s, hostname)) 
		{
			LOG_ERROR("OpenSSLManager: failed to SetCertVerificationHostname.");
		}
	}

	static void SSL_SetBio(SSL* s, BIO* read, BIO* write)
	{
		SSL_set_bio(s, read, write);
	}

	static int Shutdown()
	{
		SSL_CTX_free(ctx);
		return 0;
	}
};

#endif
