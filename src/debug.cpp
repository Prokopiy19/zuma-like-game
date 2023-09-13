#include "debug.h"

#include <algorithm>
#include <string>
#include <sstream>

#include <SDL.h>

#include "game.h"

void debug_title(SDL_Window* ptr_window)
{
    static int frames = 0;
    static float len = 0.0f;
    static Timer debug_timer;
    len += debug_timer.watch();
    debug_timer.reset();
    ++frames;
    if (len < 1.0f)
        return;
    else {
        std::ostringstream record;
        record << std::fixed;
        record.precision(1);
        record << "fps=" << frames / len;
        SDL_SetWindowTitle(ptr_window, record.str().c_str());
        len = 0.0f;
        frames = 0;
    }
}