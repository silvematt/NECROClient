// Include Standard Libraries
#include <iostream>

// Include NECRO
#include "NECROEngine.h"

// Undefine SDL_main
#undef main

int main()
{
    SDL_Log("Booting up NECROEngine...");

    if (engine.Init() == 0)
    {
        engine.Update();
    }

    SDL_Log("Exting application.");

    return 0;
}
