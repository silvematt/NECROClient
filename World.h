#ifndef NECROWORLD_H
#define NECROWORLD_H

#include "Cell.h"

const int WORLD_WIDTH  = 16;
const int WORLD_HEIGHT = 16;

const int WORLD_RENDER_OFFSET_X = 960;
const int WORLD_RENDER_OFFSET_Y = 340;

//-------------------------------------------------
// A World represents a level or a map with its
// cells and entities that lives in them.
// 
// The World is composed of a 2D array of Cells.
//-------------------------------------------------
class World
{
private:
	Cell worldmap[WORLD_WIDTH][WORLD_HEIGHT];
	Cell* worldCursor = nullptr;					// The cell the mouse is currently hovering on (if any)
	SDL_Texture* worldCursorTexture;

public:
	void			InitializeWorld();
	void			Update();
	void			Draw();
};

#endif
