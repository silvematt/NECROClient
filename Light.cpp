#include "Light.h"

#include "NECROEngine.h"

//-----------------------------------------------------------------
// Initializes max and min intensity
//-----------------------------------------------------------------
void Light::Init()
{
	maxIntensity = intensity;
	minIntensity = maxIntensity / minIntensityDivider;
}

//-----------------------------------------------------------------
// Light Update
//-----------------------------------------------------------------
void Light::Update()
{
    if (doAnim)
        Animate();
}

//-----------------------------------------------------------------
// Animates the light 
//-----------------------------------------------------------------
void Light::Animate()
{
    float deltaTime = engine.GetDeltaTime();

    float direction = goUp ? 1.0f : -1.0f;
    intensity += direction * animSpeed * deltaTime;

    // Toggle directions (pulse light)
    if (intensity <= minIntensity)
    {
        intensity = minIntensity;
        goUp = true;
        goDown = false;
    }
    else if (intensity >= maxIntensity)
    {
        intensity = maxIntensity;
        goUp = false;
        goDown = true;
    }
}

//-----------------------------------------------------------------
// Sets doAnim 
//-----------------------------------------------------------------
void Light::SetAnim(bool v)
{
    doAnim = v;
}
