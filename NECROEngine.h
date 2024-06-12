#ifndef NECROENGINE_H
#define NECROENGINE_H

#include "SDL.h"

#include "Game.h"

#include "Input.h"
#include "AssetsManager.h"
#include "Renderer.h"
#include "Console.h"

#include "Math.h"

class NECROEngine
{
private:
	// Status
	bool isRunning;
	double deltaTime;
	uint32_t lastUpdate;
	float fps;

	// Game
	NECROGame game;

	// Subsystems
	NECROInput			input;
	NECRORenderer		renderer;
	NECROAssetsManager	assetsManager;
	NECROConsole		console;
	
public:
	NECROGame&				GetGame();

	NECROInput&				GetInput();
	NECRORenderer&			GetRenderer();
	NECROAssetsManager&		GetAssetsManager();
	NECROConsole&			GetConsole();

	const double			GetDeltaTime() const;
	const float				GetFPS() const;

	int						Init();
	void					Update();
	void					Stop();
	int						Shutdown();
};


// Global access for the Engine 
extern NECROEngine engine;

// Inline functions
inline NECROGame& NECROEngine::GetGame()
{
	return game;
}

inline NECROInput& NECROEngine::GetInput()
{
	return input;
}

inline NECRORenderer & NECROEngine::GetRenderer() 
{
	return renderer;
}

inline NECROAssetsManager& NECROEngine::GetAssetsManager()
{
	return assetsManager;
}

inline NECROConsole& NECROEngine::GetConsole()
{
	return console;
}

inline const double NECROEngine::GetDeltaTime() const
{
	return deltaTime;
}

inline const float NECROEngine::GetFPS() const
{
	return fps;
}

#endif
