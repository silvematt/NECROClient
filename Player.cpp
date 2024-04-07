#include "Player.h"

#include "NECROEngine.h"
#include "Game.h"

// Initialize static member
uint32_t Player::ENT_ID = 0;

Player::~Player()
{
	// Set cur player to nullptr in NECROGame when destroyed
	engine.GetGame().SetCurPlayer(nullptr);
}

//-------------------------------------------------
// Initialize the player
//-------------------------------------------------
void Player::Init()
{
	// Construct Animator
	anim.Init(this);
	anim.AddState("idle", engine.GetAssetsManager().GetImage("player_war_idle.png"), 75);
	anim.AddState("run", engine.GetAssetsManager().GetImage("player_war_run.png"), 75);
	anim.AddState("aim_stand", engine.GetAssetsManager().GetImage("player_war_aim_stand.png"), 75);
	anim.AddState("aim_walk", engine.GetAssetsManager().GetImage("player_war_aim_walk.png"), 100);
	anim.AddState("aim_strafe", engine.GetAssetsManager().GetImage("player_war_aim_strafe.png"), 100);

	// Set default
	anim.Play("idle");

	// Construct Collider
	coll.Init(true, this, 0, 0, 32, 16);
	coll.SetOffset(0, -16);

	occlModifierX = 100;
	occlModifierY = 75;
}

//-------------------------------------------------
// Updates the Player
//-------------------------------------------------
void Player::Update()
{
	int oldGridPosX = gridPosX;
	int oldGridPosY = gridPosY;

	UpdateCloseEntities();
	HandleMovements();
	HandleAnim();

	// Update the entity base, pos, gridPos, isoPos etc.
	Entity::Update();

	if (oldGridPosX != gridPosX || oldGridPosY != gridPosY)
	{
		nextOwner = owner->GetWorld()->GetCellAt(gridPosX, gridPosY);
		TransferToCellQueue(nextOwner); // will be done as soon as the world update is finished
	}
}

void Player::HandleMovements()
{
	wasMoving = isMoving;
	wasAiming = isAiming;

	NECROInput& input = engine.GetInput();

	deltaX = 0.0f; 
	deltaY = 0.0f;

	if (controlsEnabled)
	{
		// Get input
		if (input.GetKeyHeld(SDL_SCANCODE_W))
			deltaX -= 1;
		if (input.GetKeyHeld(SDL_SCANCODE_S))
			deltaX += 1;
		if (input.GetKeyHeld(SDL_SCANCODE_D))
			deltaY -= 1;
		if (input.GetKeyHeld(SDL_SCANCODE_A))
			deltaY += 1;

		isMoving = (deltaX != 0.0f || deltaY != 0.0f) ? true : false;
		isAiming = input.GetMouseHeld(static_cast<SDL_Scancode>(SDL_BUTTON_RIGHT));
	}

	// Calculate direction
	if (!isAiming)
		CalculateIsoDirection(deltaX, deltaY);
	else
		CalculateIsoDirectionWhileAiming();

	// Select tile from tileset (used for rendering)
	tilesetYOff = isoDirection;		// pick y offset in base of direction

	// Normalize the input
	Vector2 moveInput(deltaX, deltaY);
	moveInput.Normalize();

	// Convert to Isometric
	// divison CELL_ components are inverted to equalize the movement
	NMath::CartToIso(moveInput.x / CELL_HEIGHT, moveInput.y / CELL_WIDTH, moveInput.x, moveInput.y);

	// Set speed
	curMoveSpeed = isAiming ? PLAYER_MOVE_SPEED_AIM : PLAYER_MOVE_SPEED_FREE;

	// TODO: Instead of checking against all the entities in the world, only check against the one close to the player
	float moveAmountX = moveInput.x * curMoveSpeed * engine.GetDeltaTime();
	float moveAmountY = moveInput.y * curMoveSpeed * engine.GetDeltaTime();

	// Checking is performed on each axis to allow the player to slide on the colliders
	pos.x += moveAmountX;
	coll.Update();

	for (auto const& e : closeEntities)
	{
		if (coll.TestIntersection(&e->GetCollider()))
			pos.x -= moveAmountX;
	}

	pos.y += moveAmountY;
	coll.Update();

	for (auto const& e : closeEntities)
	{
		if (coll.TestIntersection(&e->GetCollider()))
			pos.y -= moveAmountY;
	}

	coll.Update();
}


//---------------------------------------------------------------------------------------
// Updates the closeEntities vector
//---------------------------------------------------------------------------------------
void Player::UpdateCloseEntities()
{
	closeEntities.clear();

	for (int x = -1; x <= 1; x++)
		for (int y = -1; y <= 1; y++)
		{
			Cell* c = owner->GetWorld()->GetCellAt(gridPosX + x, gridPosY + y);

			if (c)
			{
				for (int i = 0; i < c->GetEntitiesPtrSize(); i++)
					if(c->GetEntityPtrAt(i)->GetID() != this->ID)
						closeEntities.push_back(c->GetEntityPtrAt(i));
			}
		}
}

//---------------------------------------------------------------------------------------
// Handles the selection of animations for player's movements (free/aim mode)
//---------------------------------------------------------------------------------------
void Player::HandleAnim()
{
	// If is in aim mode
	if (isAiming)
	{
		if (!isMoving)
		{
			// If the player is not moving, just play the standing anim
			if (anim.GetCurStateNamePlaying() != "aim_stand")
				anim.Play("aim_stand");
		}
		else
		{
			 // Strafe or walk check, in base of the direction and input, compute if the player should walk or strafe
			if (isoDirection == IsoDirection::NORTH || isoDirection == IsoDirection::SOUTH)
			{
				// Check strafe
				if (deltaY != 0.0f)
				{
					if (anim.GetCurStateNamePlaying() != "aim_strafe")
						anim.Play("aim_strafe");
				}
				else
					if (anim.GetCurStateNamePlaying() != "aim_walk")
						anim.Play("aim_walk");
			}
			else if (isoDirection == IsoDirection::NORTH_EAST || isoDirection == IsoDirection::SOUTH_WEST)
			{
				// Check strafe
				if (fabs(deltaY - deltaX) != 0.0f)
				{
					if (anim.GetCurStateNamePlaying() != "aim_strafe")
						anim.Play("aim_strafe");
				}
				else
					if (anim.GetCurStateNamePlaying() != "aim_walk")
						anim.Play("aim_walk");
			}
			else if (isoDirection == IsoDirection::EAST || isoDirection == IsoDirection::WEST)
			{
				// Check strafe
				if (deltaX != 0.0f)
				{
					if (anim.GetCurStateNamePlaying() != "aim_strafe")
						anim.Play("aim_strafe");
				}
				else
					if (anim.GetCurStateNamePlaying() != "aim_walk")
						anim.Play("aim_walk");
			}
			else if (isoDirection == IsoDirection::NORTH_WEST || isoDirection == IsoDirection::SOUTH_EAST)
			{
				if (fabs(deltaY - deltaX) < 2.0f)
				{
					if (anim.GetCurStateNamePlaying() != "aim_strafe")
						anim.Play("aim_strafe");
				}
				else
					if (anim.GetCurStateNamePlaying() != "aim_walk")
						anim.Play("aim_walk");
			}
		}
	}
	else
	{
		if (isMoving)
		{
			if (anim.GetCurStateNamePlaying() != "run")
				anim.Play("run");
		}
		else
		{
			if (anim.GetCurStateNamePlaying() != "idle")
				anim.Play("idle");
		}
	}

	anim.Update();
}

//-------------------------------------------------
// Given the input, set isoDirection
//-------------------------------------------------
void Player::CalculateIsoDirection(float deltaX, float deltaY)
{
	// Determine the direction of the player in base of the input
	// TODO: may have to delay this by few ms because at high framerates it's hard to keep an oblique direction
	if (deltaX < 0.0f)
	{
		if (deltaY < 0.0f)
			isoDirection = IsoDirection::NORTH_EAST;
		else if (deltaY > 0.0f)
			isoDirection = IsoDirection::NORTH_WEST;
		else
			isoDirection = IsoDirection::NORTH;
	}
	else if (deltaX > 0.0f)
	{
		if (deltaY < 0.0f)
			isoDirection = IsoDirection::SOUTH_EAST;
		else if (deltaY > 0.0f)
			isoDirection = IsoDirection::SOUTH_WEST;
		else
			isoDirection = IsoDirection::SOUTH;
	}
	else if (deltaY < 0.0f)
	{
		if (deltaX == 0.0f)
			isoDirection = IsoDirection::EAST;
	}
	else if (deltaY > 0.0f)
	{
		if (deltaX == 0.0f)
			isoDirection = IsoDirection::WEST;
	}
}

//-------------------------------------------------
// Set isoDirection in aiming mode
//-------------------------------------------------
void Player::CalculateIsoDirectionWhileAiming()
{
	NECROInput& input = engine.GetInput();

	Camera* curCamera = engine.GetGame().GetMainCamera();
	float curZoom = curCamera->GetZoom();

	// Get relative mouse pos 
	Vector2 playerScreenPos;
	playerScreenPos.x = isoPos.x + (img->GetTilesetWidth() / 2);
	playerScreenPos.y = isoPos.y + (img->GetTilesetHeight() / 2);

	// Calculate the angle (while accounting for zoom)
	float angle = atan2f(playerScreenPos.y - (input.GetMouseY() / curZoom), playerScreenPos.x - (input.GetMouseX() / curZoom));

	// Use degrees, wrap around negatives
	angle = angle * (180.0 / M_PI);
	if (angle < 0)
		angle += 360.0;

	angle = fmod(angle, 360.0f);

	// Determine direction based on angle
	if ((angle >= 337.5f && angle <= 360.0f) || (angle >= 0.0f && angle < 22.5f))
		isoDirection = IsoDirection::WEST;
	else if (angle >= 22.5f && angle < 67.5f)
		isoDirection = IsoDirection::NORTH_WEST;
	else if (angle >= 67.5f && angle < 112.5f)
		isoDirection = IsoDirection::NORTH;
	else if (angle >= 112.5f && angle < 157.5f)
		isoDirection = IsoDirection::NORTH_EAST;
	else if (angle >= 157.5f && angle < 202.5f)
		isoDirection = IsoDirection::EAST;
	else if (angle >= 202.5f && angle < 247.5f)
		isoDirection = IsoDirection::SOUTH_EAST;
	else if (angle >= 247.5f && angle < 292.5f)
		isoDirection = IsoDirection::SOUTH;
	else
		isoDirection = IsoDirection::SOUTH_WEST;
}
