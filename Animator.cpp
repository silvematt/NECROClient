#include "Animator.h"

//------------------------------------------------------------------
// Sets the owner of this Animator
//------------------------------------------------------------------
void Animator::SetOwner(Entity* ow)
{
    owner = ow;
}

//------------------------------------------------------------------
// Adds a state in the Animator
//------------------------------------------------------------------
void Animator::AddState(const std::string& sName, Image* sImg, float sSpeed)
{
	// Add a new state in the states map
	states.emplace(sName, AnimState(sName, sImg, sSpeed));
}

//------------------------------------------------------------------
// Updates the animator
// 
// TODO: Along with Play(), properly handle frame selection 
// (dont use SDL_GetTicks but a timer for the animation)
//------------------------------------------------------------------
void Animator::Update()
{
    owner->tilesetXOff = ((int)floor(SDL_GetTicks() / curStatePlaying->GetSpeed()) % curStatePlaying->GetImg()->GetTileset().tileXNum);
}

//------------------------------------------------------------------
// Plays the state passed as parameter (if exists)
// 
// TODO: Along with Update(), properly handle frame selection and
// allow to reset the animation if Play() is called two times on the
// same state.
//------------------------------------------------------------------
void Animator::Play(const std::string& sName)
{
    // Check if the state exists in the map
    if (states.find(sName) != states.end())
    {
        // Check if the img pointer is not null
        if (states.at(sName).GetImg() != nullptr)
        {
            curStatePlaying = &states.at(sName);
            owner->SetImg(curStatePlaying->GetImg()); // Set IMG of the entity
        }
        else
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ANIMATOR Error: Img PTR is null for state: %s ", sName.c_str());
    }
    else // State doesn't exist in map
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ANIMATOR Error: State %s not found in the states map", sName.c_str());
}
