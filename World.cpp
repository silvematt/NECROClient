#include "World.h"

#include "NECROEngine.h"
#include "Entity.h"
#include "Player.h"

Camera* curCamera;

//------------------------------------------------------------
// Initializes the World, TODO: will be loading the world file
//------------------------------------------------------------
void World::InitializeWorld()
{
	worldCursor = nullptr;
	worldCursorTexture = engine.GetAssetsManager().GetImage("tile_highlighted.png")->GetSrc();

	for(int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Initialize the Cells
			Cell& currentCell = worldmap[x][y];

			currentCell.SetWorld(this);
			currentCell.SetCellCoordinates(x, y);

			currentCell.SetBaseTexture(engine.GetAssetsManager().GetImage("tile.png")->GetSrc());

			// For testing, have a 10% probability for a cell to spawn an entity
			int r = rand() % 100;
			if (r < 10)
			{
				// Add an entity random
				std::unique_ptr<Entity> p(new Entity(Vector2(static_cast<float>(x * CELL_WIDTH), static_cast<float>(y * CELL_HEIGHT)), engine.GetAssetsManager().GetImage("tree.png")));
				currentCell.AddEntity(std::move(p));
			}
		}

	// Add a player, just for testing
	std::unique_ptr<Player> p(new Player());
	p->SetImg(engine.GetAssetsManager().GetImage("player_war_idle.png"));
	p->pos = Vector2(static_cast<float>(0 * CELL_WIDTH), static_cast<float>(0 * CELL_HEIGHT));
	p->Init();
	worldmap[0][0].AddEntity(std::move(p));

	// Set camera
	curCamera = engine.GetGame().GetMainCamera();
	curCamera->pos.x = SCREEN_WIDTH  / 2;
	curCamera->pos.y = SCREEN_HEIGHT / 2;
	curCamera->SetZoom(CAMERA_DEFAULT_ZOOM);
}

//------------------------------------------------------------
// Updates the world
//------------------------------------------------------------
void World::Update()
{
	// Compute selected cell
	int selectedCellX = 0, selectedCellY = 0;

	Vector2 mousePos(static_cast<float>(engine.GetInput().GetMouseX()), static_cast<float>(engine.GetInput().GetMouseY()));
	Vector2 mouseInWorld = curCamera->ScreenToWorld(mousePos);

	selectedCellX = mouseInWorld.x;
	selectedCellY = mouseInWorld.y;

	if (selectedCellX >= 0 && selectedCellX < WORLD_WIDTH && selectedCellY >= 0 && selectedCellY < WORLD_HEIGHT)
	{
		worldCursor = &worldmap[selectedCellX][selectedCellY];

		if (engine.GetInput().GetMouseDown(static_cast<SDL_Scancode>(SDL_BUTTON_LEFT)))
		{
			if (worldCursor->GetEntitiesSize() == 0)
			{
				// Add an entity
				std::unique_ptr<Entity> p(new Entity(Vector2(static_cast<float>(worldCursor->GetCellX() * CELL_WIDTH), static_cast<float>(worldCursor->GetCellY() * CELL_HEIGHT)), engine.GetAssetsManager().GetImage("tree.png")));
				worldCursor->AddEntity(std::move(p));
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
	// Draw the world on the main ntarget
	engine.GetRenderer().SetRenderTarget(NECRORenderer::ERenderTargets::MAIN_TARGET);
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
		engine.GetRenderer().DrawImageDirectly(worldCursorTexture, NULL, &worldCursor->GetDstRect());

	// Draw the UI on the overlay target
	engine.GetRenderer().SetRenderTarget(NECRORenderer::ERenderTargets::OVERLAY_TARGET);
	// Draw some text
	std::string textSelCell = "Selected Cell: ";
	if (worldCursor)
		textSelCell = textSelCell + "(" + std::to_string(worldCursor->GetCellX()) + ", " + std::to_string(worldCursor->GetCellY()) + ")";

	engine.GetRenderer().DrawTextDirectly(engine.GetAssetsManager().GetFont("defaultFont"), textSelCell.c_str(), 10, 10, colorRed);
}
