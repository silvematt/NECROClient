#include <iostream>

#include "NECROEngine.h"

NECROEngine engine;

//--------------------------------------
// Initializes the engine and subsystems
//--------------------------------------
int NECROEngine::Init()
{
	SDL_Log("Initializing engine...\n");

	// Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
		return -1;
	}

	// Initialize Input SubSystem
	if (input.Init() != 0)
	{
		SDL_Log("Failed to Initialize Input SubSystem\n");
		return -4;
	}

	// Initialize Renderer Subsystem
	if (renderer.Init() != 0)
	{
		SDL_Log("Failed to Initialize Renderer SubSystem\n");
		return -5;
	}

	SDL_Log("Initializing done.\n");
	return 0;
}

//--------------------------------------
// Shuts down the engine and subsystems
//--------------------------------------
int NECROEngine::Shutdown()
{
	SDL_Log("Shutting down the engine...");

	// Shutdown subsystem
	renderer.Shutdown();

	SDL_Quit();

	return 0;
}

//--------------------------------------
// Stops the engine at the next Update
//--------------------------------------
void NECROEngine::Stop()
{
	SDL_Log("Stopping the engine...");

	isRunning = false;
}

//--------------------------------------
// Engine Update
//--------------------------------------
void NECROEngine::Update()
{
	SDL_Log("Engine is running.");

	isRunning = true;

	// Engine Loop
	while (isRunning)
	{
		input.Handle();
		
		renderer.Clear();
		renderer.Show();
	}

	Shutdown();
}
