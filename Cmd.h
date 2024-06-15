#ifndef NECROCMD_H
#define NECROCMD_H

#include <vector>
#include <string>

//----------------------------------------------------------------------------------------------
// Wraps a command that can be run in the console.
//----------------------------------------------------------------------------------------------
class Cmd
{
public:
	int (Cmd::* routine)(std::vector<std::string>);

public:
	Cmd(int (Cmd::* r)(std::vector<std::string>)) :
		routine(r)
	{
	}

	int		Execute(std::vector<std::string> args);

	int		Cmd_TeleportToGrid(std::vector<std::string> args);
	int		Cmd_NoClip(std::vector<std::string> args);
};

#endif
