#include "Renderer.h"

// Define color shortcuts
const SDL_Color colorBlack = { 0, 0, 0, SDL_ALPHA_OPAQUE };
const SDL_Color colorGreen = { 0, 255, 0, SDL_ALPHA_OPAQUE };
const SDL_Color colorRed = { 255, 0, 0, SDL_ALPHA_OPAQUE };

//--------------------------------------
// Initialize
//--------------------------------------
int NECRORenderer::Init()
{
	// Create Window
	Uint32 winFlags = 0;
	//Uint32 winFlags = SDL_WINDOW_FULLSCREEN;
	window = SDL_CreateWindow("NECROEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, winFlags);

	if (!window)
		return -1;

	// Create Renderer
	Uint32 renFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
	innerRenderer = SDL_CreateRenderer(window, -1, renFlags);

	if (!innerRenderer)
		return -2;

	// Initialize the Draw Color to black
	SDL_SetRenderDrawColor(innerRenderer, colorBlack.r, colorBlack.g, colorBlack.b, colorBlack.a);

	return 0;
}

//--------------------------------------
// Shuts down the Rendering SubSystem
//--------------------------------------
int NECRORenderer::Shutdown()
{
	if (innerRenderer)
		SDL_DestroyRenderer(innerRenderer);

	if (window)
		SDL_DestroyWindow(window);

	return 0;
}

//--------------------------------------
// Render Present
//--------------------------------------
void NECRORenderer::Show()
{
	SDL_RenderPresent(innerRenderer);
}

//-----------------------------------------------------------------------------
// Update the innerRenderer with the stuff that has been drawn to the targets
// 
// TODO: Instead of calling DrawImageDirectly we'll have different SDL_Texture 
// targets and we'll SDL_RenderCopy them into the innerRenderer
//-----------------------------------------------------------------------------
void NECRORenderer::Update()
{

}

//----------------------------------------------
// Draws a Texture directly in the innerRenderer
//----------------------------------------------
void NECRORenderer::DrawImageDirectly(SDL_Texture* toDraw, const SDL_Rect* srcRect, const SDL_Rect* dstRect)
{
	//if(toDraw == nullptr)
	//	SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Nothing to draw!\n");

	SDL_RenderCopy(innerRenderer, toDraw, srcRect, dstRect);
}

//-------------------------------------------------------------------
// Draws a Text directly to the innerRenderer
// 
// TODO: For actual text rendering we can have a list of text to draw
// with their own SDL_Texture and Surface, so that we do not create
// them each frame. 
//-------------------------------------------------------------------
void NECRORenderer::DrawTextDirectly(TTF_Font* font, const char* str, int screenX, int screenY, const SDL_Color& color)
{
	SDL_Texture* renderedText = nullptr;

	SDL_Surface* textSurf = TTF_RenderText_Solid(font, str, color);
	renderedText = SDL_CreateTextureFromSurface(innerRenderer, textSurf);
	SDL_Rect dest = { screenX, screenY, textSurf->w, textSurf->h };
	
	SDL_RenderCopy(innerRenderer, renderedText, NULL, &dest);

	SDL_DestroyTexture(renderedText);
	SDL_FreeSurface(textSurf);
}

//--------------------------------------
// Clear renderer
//--------------------------------------
void NECRORenderer::Clear()
{
	SDL_RenderClear(innerRenderer);
}
