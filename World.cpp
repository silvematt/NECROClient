#include "World.h"

#include <string>

#include "NECROEngine.h"

//------------------------------------------------------------
// Initializes the World, TODO: will be loading the world file
//------------------------------------------------------------
void World::InitializeWorld()
{
	worldCursor = nullptr;

	for(int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Initialize the Cells
			Cell& currentCell = worldmap[x][y];

			currentCell.SetWorld(this);
			currentCell.SetCellCoordinates(x, y);

			currentCell.SetBaseTexture(engine.GetAssetsManager().GetImageAt(0)->GetSrc());
		}
}

void World::Update()
{
	// Compute selected cell
	int selectedCellX = 0, selectedCellY = 0;
	NMath::IsoToCart(engine.GetInput().GetMouseX() - WORLD_RENDER_OFFSET_X, engine.GetInput().GetMouseY() - WORLD_RENDER_OFFSET_Y, selectedCellX, selectedCellY);

	if (selectedCellX >= 0 && selectedCellX < WORLD_WIDTH && selectedCellY >= 0 && selectedCellY < WORLD_HEIGHT)
		worldCursor = &worldmap[selectedCellX][selectedCellY];
	else
		worldCursor = nullptr;
}

//------------------------------------------------------------
// Draws the world, TODO: will have to draw in base of visble 
// cells relative to the camera
//------------------------------------------------------------
void World::Draw()
{
	for (int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Draw the cells
			Cell& currentCell = worldmap[x][y];
			currentCell.DrawCell();
		}

	// Draw the world cursor
	if(worldCursor)
		engine.GetRenderer().DrawImageDirectly(engine.GetAssetsManager().GetImageAt(1)->GetSrc(), NULL, &worldCursor->GetDstRect());

	// Draw some text
	std::string textSelCell = "Selected Cell: ";
	if (worldCursor)
		textSelCell = textSelCell + "(" + std::to_string(worldCursor->GetCellX()) + ", " + std::to_string(worldCursor->GetCellY()) + ")";

	engine.GetRenderer().DrawTextDirectly(engine.GetAssetsManager().GetFontAt(0), textSelCell.c_str(), 10, 10, colorRed);
}
