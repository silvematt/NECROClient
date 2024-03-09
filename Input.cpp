#include "Input.h"
#include "NECROEngine.h"

//--------------------------------------
// Initialize
//--------------------------------------
int NECROInput::Init()
{
	return 0;
}

//--------------------------------------
// Handles Input by polling events
//--------------------------------------
void NECROInput::Handle()
{
	SDL_Event e;

	//Handle events
	while (SDL_PollEvent(&e) != 0)
	{
		// Quit
		if (e.type == SDL_QUIT)
		{
			engine.Stop();
		}
	}
}
