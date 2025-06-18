#include "Renderer.h"
#include <array>
#include <cmath>

#include "Math.h"
#include <SDL_image.h>

// Define color shortcuts
const SDL_Color colorBlack = { 0, 0, 0, SDL_ALPHA_OPAQUE };
const SDL_Color colorGreen = { 0, 255, 0, SDL_ALPHA_OPAQUE };
const SDL_Color colorRed = { 255, 0, 0, SDL_ALPHA_OPAQUE };
const SDL_Color colorYellow = { 255, 255, 0, SDL_ALPHA_OPAQUE };
const SDL_Color colorWhite = { 255, 255, 255, SDL_ALPHA_OPAQUE };
const SDL_Color colorGray = { 128, 128, 128, SDL_ALPHA_OPAQUE };
const SDL_Color colorPink = { 255, 20, 147, SDL_ALPHA_OPAQUE };

//--------------------------------------
// Initialize
//--------------------------------------
int NECRORenderer::Init()
{
	// Create Window
	Uint32 winFlags = 0;
	//Uint32 winFlags = SDL_WINDOW_FULLSCREEN;
	window = SDL_CreateWindow("NECROClient", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, winFlags);

	if (!window)
		return -1;

	// Create Renderer
	Uint32 renFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE;
	innerRenderer = SDL_CreateRenderer(window, -1, renFlags);

	if (!innerRenderer)
		return -2;

	// Initialize the Draw Color to black
	SDL_SetRenderDrawColor(innerRenderer, colorBlack.r, colorBlack.g, colorBlack.b, colorBlack.a);

	// Create the RenderTargets
	mainTarget.CreateMain(innerRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	overlayTarget.Create(innerRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	debugTarget.Create(innerRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetRenderTarget(ERenderTargets::MAIN_TARGET);

	exportThisFrame = false;

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
//-----------------------------------------------------------------------------
void NECRORenderer::Update()
{
	// If there was a cmd issued to export render targe textures separately, we need to do it now before we start overlapping them
	if (exportThisFrame)
		ExportTargetsSeparate();

	// Actual NECRORenderer::Update
	SetRenderTarget(MAIN_TARGET);

	// Copy the other targets on the (default) mainTarget
	SDL_RenderCopy(innerRenderer, overlayTarget.GetTargetTexture(), NULL, NULL);
	SDL_RenderCopy(innerRenderer, debugTarget.GetTargetTexture(), NULL, NULL);

	// Export final image as well
	if (exportThisFrame)
	{
		ExportComposedFinalImage();
		exportThisFrame = false;
	}
}

//----------------------------------------------
// Calls SDL_RenderSetScale on the innerRenderer
//----------------------------------------------
void NECRORenderer::SetScale(float scaleX, float scaleY)
{
	SDL_RenderSetScale(innerRenderer, scaleX, scaleY);
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

//----------------------------------------------------------------------------------
// From the Rect, convert the points in isometric and draw them as a iso-box
//----------------------------------------------------------------------------------
void NECRORenderer::DrawIsoBox(SDL_Rect* rect, SDL_Color color, float cameraOffsetX, float cameraOffsetY, float cameraZoom) // offset are camera offset
{
	// To correctly use SDL_RenderDrawLines, manually scale the coordinates based on the zoom level
	float previousScaleX = 0, previousScaleY = 0;
	SDL_RenderGetScale(innerRenderer, &previousScaleX, &previousScaleY);

	// Set Scale to 1
	SetScale(1.0, 1.0);

	std::array<SDL_Point, 5> points;

	// Define the 4 corners of the rectangle
	points[0] = { rect->x, rect->y };
	points[1] = { rect->x + rect->w, rect->y };
	points[2] = { rect->x + rect->w, rect->y + rect->h };
	points[3] = { rect->x, rect->y + rect->h };
	points[4] = points[0];  // Closing the loop

	// Convert to isometric and translate with camera, while accounting for zoom level
	for (size_t i = 0; i < points.size(); ++i)
	{
		float x = static_cast<float>(points[i].x);
		float y = static_cast<float>(points[i].y);

		NMath::CartToIso(x / CELL_WIDTH, y / CELL_HEIGHT, x, y);

		// Account for camera offset
		x += cameraOffsetX;
		y += cameraOffsetY;

		// Apply zoom level
		x *= cameraZoom;
		y *= cameraZoom;

		x = std::floor(x);
		y = std::floor(y);

		points[i] = { static_cast<int>(x), static_cast<int>(y) };
	}

	// Set render color and draw the lines
	SDL_SetRenderDrawColor(innerRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLines(innerRenderer, points.data(), points.size());
	//SDL_RenderDrawPoints(innerRenderer, points.data(), points.size());

	// Restore scale
	SetScale(previousScaleX, previousScaleY);
}

//------------------------------------------------
// Calls SDL_RenderFillRect on the inner renderer
//------------------------------------------------
void NECRORenderer::DrawRect(SDL_Rect* r, SDL_Color c)
{
	SDL_SetRenderDrawColor(innerRenderer, c.r, c.g, c.b, c.a);
	SDL_RenderFillRect(innerRenderer, r);
}

//--------------------------------------
// Clear the renderer targets
//--------------------------------------
void NECRORenderer::Clear()
{
	mainTarget.Clear();
	overlayTarget.Clear();
	debugTarget.Clear();
}

//------------------------------------------------
// Sets the Render target of the innerRenderer
//------------------------------------------------
void NECRORenderer::SetRenderTarget(ERenderTargets trg)
{
	switch (trg)
	{
		case ERenderTargets::MAIN_TARGET:
			curTarget = &mainTarget;
			SDL_SetRenderTarget(innerRenderer, mainTarget.GetTargetTexture());
			break;

		case ERenderTargets::OVERLAY_TARGET:
			curTarget = &overlayTarget;
			SDL_SetRenderTarget(innerRenderer, overlayTarget.GetTargetTexture());
			break;

		case ERenderTargets::DEBUG_TARGET:
			curTarget = &debugTarget;
			SDL_SetRenderTarget(innerRenderer, debugTarget.GetTargetTexture());
			break;
	}

	curERenTarget = trg;
}


//------------------------------------------------
// Saves texture to disk, using IMG_SavePNG
//------------------------------------------------
void NECRORenderer::SaveTexture(const char* file_name, SDL_Renderer* renderer, SDL_Texture* texture)
{
	// Backup current render target
	auto previousTarget = NECRORenderer::GetCurrentERenderTargetVal();
	
	SDL_Texture* target = SDL_GetRenderTarget(renderer);

	// Set texture as render target
	if (SDL_SetRenderTarget(renderer, texture) != 0) 
	{
		SDL_Log("Failed to set render target: %s", SDL_GetError());
		return;
	}

	int width, height;

	if (texture != NULL)
	{
		if (SDL_QueryTexture(texture, NULL, NULL, &width, &height) != 0)
		{
			SDL_Log("Failed to query texture: %s", SDL_GetError());
			SDL_SetRenderTarget(renderer, target);
			return;
		}
	}
	else // if texture is null, then it's the final composed image
	{
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
	}

	SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if (!surface) 
	{
		SDL_Log("Failed to create surface: %s", SDL_GetError());
		SDL_SetRenderTarget(renderer, target);
		return;
	}

	if (SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch) != 0) 
	{
		SDL_Log("Failed to read pixels: %s", SDL_GetError());
		SDL_FreeSurface(surface);
		SDL_SetRenderTarget(renderer, target);
		return;
	}

	if (IMG_SavePNG(surface, file_name) != 0)
		SDL_Log("Failed to save PNG: %s", SDL_GetError());

	SDL_FreeSurface(surface);

	// Restore previous render target
	SetRenderTarget(previousTarget);
}


//-----------------------------------------------------------------------------------------------
// Exports the render targets as separate images, needs to be done before Renderer.Update()
// so before the main target starts to get layered with the others
//-----------------------------------------------------------------------------------------------
void NECRORenderer::ExportTargetsSeparate()
{
	SaveTexture("main.png", innerRenderer, NULL);
	SaveTexture("overlay.png", innerRenderer, overlayTarget.GetTargetTexture());
	SaveTexture("debug.png", innerRenderer, debugTarget.GetTargetTexture());
}

//-----------------------------------------------------------------------------------------------
// Exports the final composed image, in order to work, this needs to be done AFTER Renderer.Update()
//-----------------------------------------------------------------------------------------------
void NECRORenderer::ExportComposedFinalImage()
{
	SaveTexture("final.png", innerRenderer, NULL);
}
