#ifndef NECROANIMATOR_H
#define NECROANIMATOR_H

#include <string>
#include <vector>
#include <map>

#include "Entity.h"
#include "AnimState.h"

class Animator
{
private:
	float animTime = 0;

	Entity* owner;

	std::map<std::string, AnimState> states;

	AnimState* curStatePlaying = nullptr;			// Keeps the state of the currently active state
	std::string curStateNamePlaying = "NULL";		

public:
	void Init(Entity* pOwner);

	void AddState(const std::string& sName, Image* sImg, float sSpeed);
	void Play(const std::string& sName);

	void Update();

	std::string GetCurStateNamePlaying() const;
};

inline std::string Animator::GetCurStateNamePlaying() const
{
	return curStateNamePlaying;
}

#endif
