#include "debug.h"

#include <string>

#include <SDL.h>

#include "game.h"

void debug_title(SDL_Window* ptr_window)
{
    std::string title = "Current projectiles: ";
    title += std::to_string(state.projectiles.size());
    SDL_SetWindowTitle(ptr_window, title.c_str());
}