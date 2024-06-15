#include "Console.h"
#include "NECROEngine.h"
#include "Cmd.h"

#include <sstream>

//-------------------------------------------------------
// Initializes the console
//-------------------------------------------------------
int NECROConsole::Init()
{
	inputField.Init(SDL_Rect{ 0,0,309,52}, SDL_Rect{ 25, SCREEN_HEIGHT - 120, 380,35 }, "", engine.GetAssetsManager().GetImage("default_input_field.png"), engine.GetAssetsManager().GetImage("default_active_input_field.png"), 0);

	cmds.insert({ "teleport", Cmd(&Cmd::Cmd_TeleportToGrid)});
	cmds.insert({ "noclip", Cmd(&Cmd::Cmd_NoClip) });

	return 0;
}

//-------------------------------------------------------
// Toggles the console, handles send command as well
//-------------------------------------------------------
void NECROConsole::Toggle()
{
	// If the console is active, see if we have to send cmd or close the console
	if (active)
	{
		if (inputField.str.size() > 0)
		{
			// If the command is executed, close the console after sending it
			int val = SendCmd(inputField.str);
			if (val == 0)
				active = false;
		}
		else
			active = false;
	}
	else
	{
		active = true;
		curLineOffset = 0;
		curHistoryIndex = -1;
	}

	inputField.SetFocused(active);
}


//-------------------------------------------------------
// Update routine
//-------------------------------------------------------
void NECROConsole::Update()
{
	NECROInput& i = engine.GetInput();

	if (i.GetKeyDown(SDL_SCANCODE_RETURN) || i.GetKeyDown(SDL_SCANCODE_KP_ENTER))
		Toggle();

	if (active)
	{
		if (i.GetKeyDown(SDL_SCANCODE_PAGEUP))
			curLineOffset += CONSOLE_MAX_LINES_PER_PAGE;
		else if (i.GetKeyDown(SDL_SCANCODE_PAGEDOWN))
			curLineOffset -= CONSOLE_MAX_LINES_PER_PAGE;

		curLineOffset = SDL_clamp(curLineOffset, 0, history.size());

		// Browse history with arrows
		if (i.GetKeyDown(SDL_SCANCODE_UP))
		{
			curHistoryIndex += 1;
			curHistoryIndex = SDL_clamp(curHistoryIndex, 0, history.size() - 1);

			int index = history.size() - 1 - curHistoryIndex;
			if (index >= 0 && index <= history.size() - 1)
				inputField.str = history[index];

		}
		else if (i.GetKeyDown(SDL_SCANCODE_DOWN))
		{
			curHistoryIndex -= 1;
			curHistoryIndex = SDL_clamp(curHistoryIndex, 0, history.size() - 1);

			int index = history.size() - 1 - curHistoryIndex;
			if (index >= 0 && index <= history.size() - 1)
				inputField.str = history[index];
		}

		Draw();
	}
}

//-------------------------------------------------------
// Draws the console on the Overlay target
//-------------------------------------------------------
void NECROConsole::Draw()
{
	engine.GetRenderer().SetRenderTarget(NECRORenderer::OVERLAY_TARGET);
	inputField.Draw();

	int count = 1;
	for (int i = logs.size()-1; i >= 0; i--)
	{
		int actualOffset = 0;

		if(i-curLineOffset < logs.size())
			engine.GetRenderer().DrawTextDirectly(engine.GetAssetsManager().GetFont("defaultFont"), logs[i - curLineOffset].c_str(), 25, (SCREEN_HEIGHT - 120) - 40 * count, colorWhite);

		// Limit console
		if (count >= CONSOLE_MAX_LINES_PER_PAGE)
			break;

		count++;
	}
}

//-------------------------------------------------------
// Adds the str to the console history
//-------------------------------------------------------
void NECROConsole::Log(const std::string& str)
{
	logs.push_back(str);
}

//-------------------------------------------------------
// Calls when a command is written and sent
// 
// RETURNS Cmd return value if the command is run
// RETURNS 1 if the command was not found and just logged
//-------------------------------------------------------
int NECROConsole::SendCmd(const std::string& cmd)
{
	// Add command to history
	history.push_back(cmd);
	
	Log("> " + cmd); // log command

	// Get the input
	std::vector<std::string> input = SplitInput(cmd);

	// Process it
	if (input.size() > 0)
	{
		// Input[0] is the function name

		auto it = cmds.find(input[0]);
		if (it != cmds.end())
		{
			// Command found, execute it
			return cmds.at(input[0]).Execute(input);
		} // ELSE command not registered, just keep it logged
	}

	return 1;
}

//-----------------------------------------------------------------------
// Given a space-sperated string, splits it into a vector of strings
//-----------------------------------------------------------------------
std::vector<std::string> NECROConsole::SplitInput(const std::string& input)
{
	std::vector<std::string> tokens;
	std::istringstream stream(input);
	std::string token;

	while (stream >> token) 
		tokens.push_back(token);

	return tokens;
}
