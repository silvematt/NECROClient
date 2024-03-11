#include "Game.h"

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
	currentWorld.Draw();
}

//--------------------------------------
// Shuts down game related things
//--------------------------------------
void NECROGame::Shutdown()
{

}
