#include "Player.h"

#include "NECROEngine.h"

void Player::Update()
{
	float deltaX = 0;
	float deltaY = 0;

	NECROInput& input = engine.GetInput();

	if (input.GetKeyHeld(SDL_SCANCODE_W))
		deltaY -= 1;
	if (input.GetKeyHeld(SDL_SCANCODE_S))
		deltaY += 1;
	if (input.GetKeyHeld(SDL_SCANCODE_D))
		deltaX += 1;
	if (input.GetKeyHeld(SDL_SCANCODE_A))
		deltaX -= 1;

	pos.x += deltaX;
	pos.y += deltaY;

	Entity::Update();
}
