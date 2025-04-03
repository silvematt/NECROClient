#ifndef AUTH_CODES_H
#define AUTH_CODES_H


//----------------------------------------------------------------------------------------------------
// Define packets structures
//----------------------------------------------------------------------------------------------------
enum AuthPacketIDs
{
	PCKTID_AUTH_LOGIN_GATHER_INFO = 0x00,
	PCKTID_AUTH_LOGIN_ATTEMPT = 0x01,
	PCKTID_AUTH_CONFIRM = 0x02
};

//--------------------------------------------------------------------------------------------
// Results to send as payload to tell the client what happened as a result of the command 
//--------------------------------------------------------------------------------------------
enum AuthResults
{
	AUTH_SUCCESS						= 0x00,
	AUTH_FAILED_UNKNOWN_ACCOUNT			= 0x01,
	AUTH_FAILED_ACCOUNT_BANNED			= 0x02,
	AUTH_FAILED_WRONG_PASSWORD			= 0x03,
	AUTH_FAILED_WRONG_CLIENT_VERSION	= 0x04,
	AUTH_FAILED_USERNAME_IN_USE			= 0x05		// before we implement database and we'll just have username uniqueness per session
};


#endif
