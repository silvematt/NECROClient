#include "Console.h"
#include "NECROEngine.h"

int NECROConsole::Init()
{
	inputField.Init(SDL_Rect{ 0,0,309,52}, SDL_Rect{ 25, SCREEN_HEIGHT - 120, 380,35 }, "", engine.GetAssetsManager().GetImage("default_input_field.png"), engine.GetAssetsManager().GetImage("default_active_input_field.png"), 0);
	return 0;
}

void NECROConsole::Toggle()
{
	active = !active;
	curLineOffset = 0;
	curHistoryIndex = -1;
	inputField.SetFocused(active);

	// Send command if(!active)
	if (!active)
	{
		if (inputField.str.size() > 0)
			history.push_back(inputField.str);
	}
}


void NECROConsole::Update()
{
	NECROInput& i = engine.GetInput();

	if (i.GetKeyDown(SDL_SCANCODE_RETURN) || i.GetKeyDown(SDL_SCANCODE_KP_ENTER))
		Toggle();

	if (active)
	{
		if (i.GetKeyDown(SDL_SCANCODE_PAGEUP))
			curLineOffset += CONSOLE_MAX_LINES_PER_HISTORY;
		else if (i.GetKeyDown(SDL_SCANCODE_PAGEDOWN))
			curLineOffset -= CONSOLE_MAX_LINES_PER_HISTORY;

		curLineOffset = SDL_clamp(curLineOffset, 0, history.size());

		// Browse history with arrows
		if (i.GetKeyDown(SDL_SCANCODE_UP))
		{
			curHistoryIndex += 1;
			curHistoryIndex = SDL_clamp(curHistoryIndex, 0, history.size() - 1);

			int index = history.size() - 1 - curHistoryIndex;
			if (index >= 0 && index <= history.size() - 1)
				inputField.str = history[index];

		}
		else if (i.GetKeyDown(SDL_SCANCODE_DOWN))
		{
			curHistoryIndex -= 1;
			curHistoryIndex = SDL_clamp(curHistoryIndex, 0, history.size() - 1);

			int index = history.size() - 1 - curHistoryIndex;
			if (index >= 0 && index <= history.size() - 1)
				inputField.str = history[index];
		}

		Draw();
	}
}

void NECROConsole::Draw()
{
	engine.GetRenderer().SetRenderTarget(NECRORenderer::OVERLAY_TARGET);
	inputField.Draw();

	int count = 1;
	for (int i = history.size()-1; i >= 0; i--)
	{
		int actualOffset = 0;

		if(i-curLineOffset < history.size())
			engine.GetRenderer().DrawTextDirectly(engine.GetAssetsManager().GetFont("defaultFont"), history[i - curLineOffset].c_str(), 25, (SCREEN_HEIGHT - 120) - 40 * count, colorWhite);

		// Limit console
		if (count >= CONSOLE_MAX_LINES_PER_HISTORY)
			break;

		count++;
	}
}
