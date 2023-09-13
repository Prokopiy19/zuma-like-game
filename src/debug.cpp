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

    static int prev_seg = 0;
    int cnt_seg = 0;
    for (const auto& line : state.lines)
        cnt_seg += line.segments.size();

    static float fps = 0.0f;
    bool fps_change = false;
    if (len >= 1.0f) {
        fps = frames / len;
        fps_change = true;
        len = 0;
        frames = 0;
    } 
    if (cnt_seg != prev_seg || fps_change) {
        std::ostringstream record;
        record << "segments: " << cnt_seg << ". ";
        record << std::fixed;
        record.precision(1);
        record << "fps=" << fps;
        SDL_SetWindowTitle(ptr_window, record.str().c_str());
    }
    prev_seg = cnt_seg;
}