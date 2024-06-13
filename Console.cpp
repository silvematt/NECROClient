#include "Console.h"
#include "NECROEngine.h"

int NECROConsole::Init()
{
	inputField.Init(SDL_Rect{ 0,0,309,52}, SDL_Rect{ 25, SCREEN_HEIGHT - 120, 360,35 }, "", engine.GetAssetsManager().GetImage("default_input_field.png"), engine.GetAssetsManager().GetImage("default_active_input_field.png"), 0);
	return 0;
}

void NECROConsole::Update()
{
	if (engine.GetInput().GetKeyDown(SDL_SCANCODE_RETURN))
		Toggle();

	if (active)
	{
		Draw();
	}
}

void NECROConsole::Draw()
{
	engine.GetRenderer().SetRenderTarget(NECRORenderer::OVERLAY_TARGET);
	inputField.Draw();
}

void NECROConsole::Toggle()
{
	active = !active;

	inputField.SetFocused(active);

	// Send command if(!active)
	if (!active)
	{

	}
}
