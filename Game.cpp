#include "Game.h"
#include "NECROEngine.h"

std::string GameModeMap[] =
{
	"EDIT_MODE",
	"PLAY_MODE"
};

//--------------------------------------
// Initialize the game
//--------------------------------------
void NECROGame::Init()
{
	currentWorld.InitializeWorld();
}

//--------------------------------------
// Game Update
//--------------------------------------
void NECROGame::Update()
{
	HandleInput();

	mainCamera.Update();
	currentWorld.Update();
	currentWorld.Draw();
}

//--------------------------------------
// Shuts down game related things
//--------------------------------------
void NECROGame::Shutdown()
{

}

//--------------------------------------
// Handles input for global game cmds
//--------------------------------------
void NECROGame::HandleInput()
{
	// Game CMDs
	NECROInput& input = engine.GetInput();

	// Switch game mode
	if (engine.GetInput().GetKeyDown(SDL_SCANCODE_TAB))
	{
		switch (curMode)
		{
			case GameMode::EDIT_MODE:
				SetCurMode(GameMode::PLAY_MODE);
			break;

			case GameMode::PLAY_MODE:
				SetCurMode(GameMode::EDIT_MODE);
			break;
		}
	}
}
