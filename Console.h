#ifndef NECROCONSOLE_H
#define NECROCONSOLE_H

#include <string>
#include <vector>

#include "InputField.h"

const int CONSOLE_MAX_STR_SIZE = 128;
const int CONSOLE_MAX_STR_STORED = 2048;  // To limit history
const float	CONSOLE_FADE_OUT_TIME = 5.0f; // after 5 seconds, the messages will fade out until the console is toggled again

//----------------------------------------------------------------------------------------------
// NECROConsole, for console commands and in-game chat
//----------------------------------------------------------------------------------------------
class NECROConsole
{
private:
	bool active;
	std::vector<std::string> history; // contains all the commands, messages and info the console logs.

	
	InputField inputField;

public:
	int			Init();

	void		Update();
	void		Draw();

	void		Toggle();
};

#endif
