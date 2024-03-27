#ifndef NECROPLAYER_H
#define NECROPLAYER_H

#include "Entity.h"
#include "Game.h"
#include "Animator.h"


const float PLAYER_MOVE_SPEED_FREE = 2.5f;
const float PLAYER_MOVE_SPEED_AIM = 1.0f;

//-------------------------------------------------
// Player class, derived by Entity
//-------------------------------------------------
class Player : public Entity
{
private:
	Animator anim;

	float curMoveSpeed = 2.5f;
	IsoDirection isoDirection = IsoDirection::WEST;			// The isometric direction the player is facing

	bool isAiming = false;									// Is the player in aim mode?
	bool isMoving = false;

	float relativeMouseX, relativeMouseY;

private:
	void			CalculateIsoDirection(float deltaX, float deltaY);
	void			CalculateIsoDirectionWhileAiming();
	void			HandleMovements();

public:
	void			Init();
	virtual void	Update() override;

	float			GetCurMoveSpeed() const;
};

inline float Player::GetCurMoveSpeed() const
{
	return curMoveSpeed;
}

#endif
