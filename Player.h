#ifndef NECROPLAYER_H
#define NECROPLAYER_H

#include "Entity.h"
#include "Game.h"

//-------------------------------------------------
// Player class, derived by Entity
//-------------------------------------------------
class Player : public Entity
{
private:
	float curMoveSpeed = 2.5f;
	IsoDirection isoDirection = IsoDirection::WEST;			// The isometric direction the player is facing


private:
	void			CalculateIsoDirection(float deltaX, float deltaY);

public:
	float			GetCurMoveSpeed() const;

	virtual void	Update() override;
};

inline float Player::GetCurMoveSpeed() const
{
	return curMoveSpeed;
}

#endif
