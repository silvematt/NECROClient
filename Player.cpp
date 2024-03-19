#include "Player.h"

#include "NECROEngine.h"

//-------------------------------------------------
// Updates the Player
//-------------------------------------------------
void Player::Update()
{
	float deltaX = 0.0f;
	float deltaY = 0.0f;

	NECROInput& input = engine.GetInput();

	// Get input
	if (input.GetKeyHeld(SDL_SCANCODE_W))
		deltaX -= 1;
	if (input.GetKeyHeld(SDL_SCANCODE_S))
		deltaX += 1;
	if (input.GetKeyHeld(SDL_SCANCODE_D))
		deltaY -= 1;
	if (input.GetKeyHeld(SDL_SCANCODE_A))
		deltaY += 1;

	// Calculate direction
	CalculateIsoDirection(deltaX, deltaY);

	// Select tile from tileset
	tilesetXOff = 0;				// no animations yet
	tilesetYOff = isoDirection;		// pick y offset in base of direction

	// Normalizet the input
	Vector2 moveInput(deltaX, deltaY);
	moveInput.Normalize();

	// Convert to Isometric
	// divison CELL_ components are inverted to equalize the movement
	NMath::CartToIso(moveInput.x / CELL_HEIGHT, moveInput.y / CELL_WIDTH, moveInput.x, moveInput.y);

	// Update orthogonal position
	pos.x += moveInput.x * curMoveSpeed;
	pos.y += moveInput.y * curMoveSpeed;

	// Update the entity base
	Entity::Update();
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