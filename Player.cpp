#include "Player.h"

#include "NECROEngine.h"

void Player::Update()
{
	float deltaX = 0;
	float deltaY = 0;

	NECROInput& input = engine.GetInput();

	if (input.GetKeyHeld(SDL_SCANCODE_W))
		deltaX -= 1;
	if (input.GetKeyHeld(SDL_SCANCODE_S))
		deltaX += 1;
	if (input.GetKeyHeld(SDL_SCANCODE_D))
		deltaY -= 1;
	if (input.GetKeyHeld(SDL_SCANCODE_A))
		deltaY += 1;

	Vector2 moveInput(deltaX, deltaY);
	moveInput.Normalize();

	// divison CELL_ components are inverted to equalize the movement
	NMath::CartToIso(moveInput.x / CELL_HEIGHT, moveInput.y / CELL_WIDTH, moveInput.x, moveInput.y);

	pos.x += moveInput.x * curMoveSpeed;
	pos.y += moveInput.y * curMoveSpeed;

	Entity::Update();
}
