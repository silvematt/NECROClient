#include "World.h"

#include <string>

#include "NECROEngine.h"
#include "Entity.h"


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

			// For testing, have a 10% probability for a cell to spawn an entity
			int r = rand() % 100;
			if (r < 10)
			{
				// Add an entity random
				Entity e;
				e.Init(Vector2(x * CELL_WIDTH, y * CELL_HEIGHT), engine.GetAssetsManager().GetImageAt(2));
				currentCell.AddEntity(e);
			}
		}
}

//------------------------------------------------------------
// Updates the world
//------------------------------------------------------------
void World::Update()
{
	// Compute selected cell
	int selectedCellX = 0, selectedCellY = 0;
	NMath::IsoToCart(engine.GetInput().GetMouseX() - WORLD_RENDER_OFFSET_X, engine.GetInput().GetMouseY() - WORLD_RENDER_OFFSET_Y, selectedCellX, selectedCellY);

	if (selectedCellX >= 0 && selectedCellX < WORLD_WIDTH && selectedCellY >= 0 && selectedCellY < WORLD_HEIGHT)
	{
		worldCursor = &worldmap[selectedCellX][selectedCellY];

		if (engine.GetInput().GetMouseDown(static_cast<SDL_Scancode>(SDL_BUTTON_LEFT)))
		{
			if (worldCursor->GetEntitiesSize() == 0)
			{
				// Add an entity
				Entity e;
				e.Init(Vector2(worldCursor->GetCellX() * CELL_WIDTH, worldCursor->GetCellY() *CELL_HEIGHT), engine.GetAssetsManager().GetImageAt(2));
				worldCursor->AddEntity(e);
			}
			else
			{
				// Remove the entity
				worldCursor->RemoveEntity(0);
			}
		}
	}
	else
		worldCursor = nullptr;

	// Update all the cells, TODO we can let the World have a vector of pointers to all the cell's entities to avoid having to look inside the worldmap
	//						 In that way, we can also mark entities as 'static' and avoid to update them each world update.
	for (int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Draw the cells
			Cell& currentCell = worldmap[x][y];
			currentCell.Update();
		}
}

//------------------------------------------------------------
// Draws the world. 
// 
// TODO: will have to draw in base of visble 
// cells relative to the camera
//------------------------------------------------------------
void World::Draw()
{
	// Draw the world base 
	for (int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Draw the cells
			Cell& currentCell = worldmap[x][y];
			currentCell.DrawCell();
		}

	// Draw the entities, TODO: same thing as said above for the Cell update
	for (int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Draw the cells
			Cell& currentCell = worldmap[x][y];
			currentCell.DrawEntities();
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
