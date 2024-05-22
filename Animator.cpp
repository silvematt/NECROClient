#include "Animator.h"

//------------------------------------------------------------------
// Initializes the Animator and set the owner of this Animator
//------------------------------------------------------------------
void Animator::Init(Entity* pOwner)
{
    owner = pOwner;
    animTime = SDL_GetTicks();
}

//------------------------------------------------------------------
// Adds a state in the Animator
//------------------------------------------------------------------
void Animator::AddState(const std::string& sName, Image* sImg, float sSpeed)
{
	// Add a new state in the states map
	states.emplace(sName, AnimState(sName, sImg, sSpeed));
}

//-------------------------------------------------------------------------------------
// Updates the animator, updates the tilesetXOffset in order to select the right frame
//-------------------------------------------------------------------------------------
void Animator::Update()
{
    if (owner && curStatePlaying)
        owner->tilesetXOff = ((int)floor((SDL_GetTicks() - animTime) / curStatePlaying->GetSpeed()) % curStatePlaying->GetImg()->GetTileset().tileXNum);
}

//------------------------------------------------------------------
// Plays the state passed as parameter (if exists)
//------------------------------------------------------------------
void Animator::Play(const std::string& sName)
{
    // Check if the state exists in the map
    if (states.find(sName) != states.end())
    {
        // Check if the img pointer is not null
        if (states.at(sName).GetImg() != nullptr)
        {
            // Set state
            curStatePlaying = &states.at(sName);
            curStateNamePlaying = sName;

            // Set anim
            owner->SetImg(curStatePlaying->GetImg()); // Set IMG of the entity
            animTime = SDL_GetTicks();
        }
        else
            SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ANIMATOR Error: Img PTR is null for state: %s ", sName.c_str());
    }
    else // State doesn't exist in map
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "ANIMATOR Error: State %s not found in the states map", sName.c_str());
}
