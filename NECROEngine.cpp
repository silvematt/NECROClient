#include <iostream>

#include "NECROEngine.h"
#include "World.h"

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
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init Error: %s\n", SDL_GetError());
		return -1;
	}

	int imgFlags = IMG_INIT_PNG;
	// Img init returns 0 on failure
	if (IMG_Init(imgFlags) == 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL IMG_Init Error\n");
		return -2;
	}

	if (TTF_Init() != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL TTF_Init Error\n");
		return -3;
	}

	// Initialize Input SubSystem
	if (input.Init() != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to Initialize Input SubSystem\n");
		return -4;
	}

	// Initialize Renderer Subsystem
	if (renderer.Init() != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to Initialize Renderer SubSystem\n");
		return -5;
	}

	// Initialize AssetsManager SubSystem
	if (assetsManager.Init() != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to Initialize AssetsManager SubSystem\n");
		return -6;
	}

	srand(time(NULL));

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

	// Shutdown SDL
	SDL_Quit();
	IMG_Quit();
	TTF_Quit();

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

	// Initialize game
	game.Init();

	// Engine Loop
	while (isRunning)
	{
		input.Handle();
		
		renderer.Clear();

		game.Update();

		renderer.Update();
		renderer.Show();
	}

	// Shutdown
	game.Shutdown();
	Shutdown();
}
