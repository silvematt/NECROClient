#include "Input.h"
#include "NECROEngine.h"

//--------------------------------------
// Initialize
//--------------------------------------
int NECROInput::Init()
{
	oldMouseX = oldMouseY = 0;

	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	// Set mouse buttons
	for (int i = 1; i <= 3; i++) 
	{
		prevMouseButtons[i - 1] = 0;
		mouseButtons[i - 1] = mouseState & SDL_BUTTON(i);
	}

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
		switch (e.type)
		{
			case SDL_QUIT:
				engine.Stop();
				break;

		}
	}

	oldMouseX = mouseX;
	oldMouseY = mouseY;

	const Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

	// Set mouse buttons
	for (int i = 1; i <= 3; i++) 
	{
		prevMouseButtons[i - 1] = mouseButtons[i - 1];
		mouseButtons[i - 1] = mouseState & SDL_BUTTON(i);
	}
}

int NECROInput::GetMouseDown(SDL_Scancode button) const
{
	if (button < SDL_BUTTON_LEFT || button > SDL_BUTTON_RIGHT)
		return -1;

	return (mouseButtons[button - 1] & ~prevMouseButtons[button - 1]);
}
