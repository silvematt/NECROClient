#ifndef NECRORENDERER_H
#define NECRORENDERER_H

#include "SDL.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

class NECRORenderer
{
private:
	// Main elements
	SDL_Window*		window;
	SDL_Renderer*	innerRenderer;

public:
	SDL_Window*		const	GetWindow() const;
	SDL_Renderer*	const	GetInnerRenderer() const;

	int						Init();
	int						Shutdown();
	void					Show();
	void					Clear();
};

#endif
