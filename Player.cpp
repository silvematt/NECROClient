#include "Player.h"

#include "NECROEngine.h"

//-------------------------------------------------
// Initialize the player
//-------------------------------------------------
void Player::Init()
{
	// Construct Animator
	anim.Init(this);
	anim.AddState("idle", engine.GetAssetsManager().GetImage("player_war_idle.png"), 75);
	anim.AddState("run", engine.GetAssetsManager().GetImage("player_war_run.png"), 75);

	// Set default
	anim.Play("idle");
}

//-------------------------------------------------
// Updates the Player
//-------------------------------------------------
void Player::Update()
{
	NECROInput& input = engine.GetInput();

	isAiming = input.GetMouseHeld(static_cast<SDL_Scancode>(SDL_BUTTON_RIGHT));

	HandleMovements();

	anim.Update();

	// Update the entity base
	Entity::Update();

}

void Player::HandleMovements()
{
	wasMoving = isMoving;

	NECROInput& input = engine.GetInput();

	float deltaX = 0.0f, deltaY = 0.0f;

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

	if (isMoving && !wasMoving)
		anim.Play("run");
	else if (!isMoving && wasMoving)
		anim.Play("idle");

	// Calculate direction
	if (!isAiming)
		CalculateIsoDirection(deltaX, deltaY);
	else
		CalculateIsoDirectionWhileAiming();

	// Select tile from tileset (used for rendering)
	tilesetYOff = isoDirection;		// pick y offset in base of direction

	// Normalizet the input
	Vector2 moveInput(deltaX, deltaY);
	moveInput.Normalize();

	// Convert to Isometric
	// divison CELL_ components are inverted to equalize the movement
	NMath::CartToIso(moveInput.x / CELL_HEIGHT, moveInput.y / CELL_WIDTH, moveInput.x, moveInput.y);

	// Set speed
	curMoveSpeed = isAiming ? PLAYER_MOVE_SPEED_AIM : PLAYER_MOVE_SPEED_FREE;

	// Update orthogonal position
	pos.x += moveInput.x * curMoveSpeed;
	pos.y += moveInput.y * curMoveSpeed;
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
