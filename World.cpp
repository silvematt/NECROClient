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

	// Add a player, just for testing (ID = 0)
	std::unique_ptr<Player> p(new Player());
	p->SetImg(engine.GetAssetsManager().GetImage("player_war_idle.png"));
	p->pos = Vector2(static_cast<float>(0 * CELL_WIDTH), static_cast<float>(0 * CELL_HEIGHT));
	p->Init();
	AddEntity(std::move(p));

	for(int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Initialize the Cells
			Cell& currentCell = worldmap[x][y];

			currentCell.SetWorld(this);
			currentCell.SetCellCoordinates(x, y);

			currentCell.SetBaseTexture(engine.GetAssetsManager().GetImage("tile.png")->GetSrc());

			// For testing, randomly spawn an entity
			int r = rand() % 100;
			if (r < 1)
			{
				// Add an entity randomly
				// TODO: prefab entities ex. AddEntity(Prefabs::Get("Tree01"))
				std::unique_ptr<Entity> p(new Entity(Vector2(static_cast<float>(x * CELL_WIDTH), static_cast<float>(y * CELL_HEIGHT)), engine.GetAssetsManager().GetImage("tree.png")));
				p->GetCollider().Init(true, p.get(), 0, 0, 64, 32); // set collision
				AddEntity(std::move(p));
			}
		}

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
			if (worldCursor->GetEntitiesPtrSize() != 0)
			{
				RemoveEntity(worldCursor->GetEntityPtrAt(0)->GetID());
			}
			else
			{
				std::unique_ptr<Entity> p(new Entity(Vector2(static_cast<float>(worldCursor->GetCellX() * CELL_WIDTH), static_cast<float>(worldCursor->GetCellY() * CELL_HEIGHT)), engine.GetAssetsManager().GetImage("tree.png")));
				p->GetCollider().Init(true, p.get(), 0, 0, 64, 32); // set collision
				AddEntity(std::move(p));
			}
		}
	}
	else
		worldCursor = nullptr;

	// Update all the cells, TODO we can let the World have a vector of pointers to all the cell's entities to avoid having to look inside the worldmap
	//						 In that way, we can also mark entities as 'static' and avoid to update them each world update.
	entitiesWaitingForTransfer.clear();
	for (int x = 0; x < WORLD_WIDTH; x++)
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			// Draw the cells
			Cell& currentCell = worldmap[x][y];
			currentCell.Update();
		}

	TransferPendingEntities();
}

//------------------------------------------------------------
// Draws the world. 
// 
// TODO: will have to draw in base of visble 
// cells relative to the camera
//------------------------------------------------------------
void World::Draw()
{
	// Draw the world on the main target
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

//------------------------------------------------------------------------
// Returns a Cell* at the coordinates specified as arguments of this call
//------------------------------------------------------------------------
Cell* World::GetCellAt(int x, int y)
{
	if (x >= 0 && x < WORLD_WIDTH && y >= 0 && y < WORLD_HEIGHT)
		return &worldmap[x][y];
	else
		return nullptr;
}

//-----------------------------------------------------------------------
// Adds the Entity to the entities map, transferring ownership
//-----------------------------------------------------------------------
void World::AddEntity(std::unique_ptr<Entity>&& e)
{
	// Add a ptr into the Cell
	e->gridPosX = e->pos.x / CELL_WIDTH;
	e->gridPosY = e->pos.y / CELL_HEIGHT;

	if (e->gridPosX >= 0 && e->gridPosX < WORLD_WIDTH && e->gridPosY >= 0 && e->gridPosY < WORLD_HEIGHT)
	{
		worldmap[e->gridPosX][e->gridPosY].AddEntityPtr(e.get());

		// Add entity in the world map
		allEntities.insert({ e->GetID(), std::move(e) });
	}
	else
	{
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "World::AddEntity(): gridPos is out of bounds. Entity will not be added to map.\n");
	}
}

void World::RemoveEntity(uint32_t atID)
{
	allEntities.at(atID)->GetOwner()->RemoveEntityPtr(atID);
	allEntities.erase(atID);
}

void World::TransferPendingEntities()
{
	for (int i = 0; i < entitiesWaitingForTransfer.size(); i++)
	{
		// nullptr as argument means Entity->nextOwner
		entitiesWaitingForTransfer[i]->TransferToCellImmediately(nullptr);
	}
}

void World::AddPendingEntityToTransfer(Entity* e)
{
	entitiesWaitingForTransfer.push_back(e);
}