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
	void					Update();
	void					Clear();

	void					DrawImageDirectly(SDL_Texture* toDraw, const SDL_Rect* srcRect, const SDL_Rect* dstRect);
};

// Color shortcuts
extern const SDL_Color colorBlack;
extern const SDL_Color colorGreen;
extern const SDL_Color colorRed;

inline SDL_Window* const NECRORenderer::GetWindow() const
{
	return window;
}

inline SDL_Renderer* const NECRORenderer::GetInnerRenderer() const
{
	return innerRenderer;
}

#endif
