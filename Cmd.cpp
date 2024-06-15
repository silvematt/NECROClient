#include "Cmd.h"
#include "NECROEngine.h"
#include "Collider.h"
#include "Player.h"

#include "SDL.h"

//----------------------------------------------------------------------------------------------
// Executes the command bound to the routine function pointer
//----------------------------------------------------------------------------------------------
int Cmd::Execute(std::vector<std::string> args)
{
	if(routine)
		return (this->*routine)(args);

	return 1; // 1 means problem
}

//----------------------------------------------------------------------------------------------
// Teleports the player to a grid pos - TODO: make this cmd take an additional entity param
//----------------------------------------------------------------------------------------------
int Cmd::Cmd_TeleportToGrid (std::vector<std::string> args)
{
	NECROConsole& c = engine.GetConsole();

	if (args.size() < 3)
	{
		c.Log("CMD_TeleportToGrid: requires 2 arguments [x, y], but only " + std::to_string(args.size() - 1) + " were passed.");
		return 1;
	}
	else
	{
		int x = Utility::TryParseInt(args[1]); 
		int y = Utility::TryParseInt(args[2]);

		// Teleport player
		engine.GetGame().GetCurPlayer()->TeleportToGrid(x, y);

		return 0;
	}
}

//----------------------------------------------------------------------------------------------
// Disables collision checking for the player
//----------------------------------------------------------------------------------------------
int Cmd::Cmd_NoClip(std::vector<std::string> args)
{
	NECROConsole& c = engine.GetConsole();
	Collider* pColl = engine.GetGame().GetCurPlayer()->GetCollider();

	pColl->enabled = !pColl->enabled;

	std::string s = "No Clip: ";
	s.append((!pColl->enabled ? "enabled" : "disabled"));
	c.Log(s);

	return 0;
}
