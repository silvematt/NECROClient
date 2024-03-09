#ifndef NECROENGINE_H
#define NECROENGINE_H

#include "SDL.h"
#undef main

#include "Input.h"
#include "Renderer.h"

class NECROEngine
{
private:
	// Status
	bool isRunning;

	// Subsystems
	NECROInput		input;
	NECRORenderer	renderer;
	
public:
	int						Init();
	void					Update();
	void					Stop();
	int						Shutdown();
};


// Global access for the Engine 
extern NECROEngine engine;

#endif
