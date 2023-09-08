#include "debug.h"

#include <algorithm>
#include <string>
#include <sstream>

#include <SDL.h>

#include "game.h"

void debug_title(SDL_Window* ptr_window, float delta)
{
    static int frames = 0;
    static float len = 0.0f;
    len += delta;
    ++frames;
    if (len < 1.0f)
        return;
    else {
        std::ostringstream record;
        record << "fps=" << std::to_string(frames / len);
        SDL_SetWindowTitle(ptr_window, record.str().c_str());
        len = 0.0f;
        frames = 0;
    }
}