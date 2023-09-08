#include "timer.h"

float Timer::watch() const
{
    if (ticking)
        return convert_to_sec(SDL_GetPerformanceCounter() - start);
    else
        return convert_to_sec(start);
}

void Timer::pause()
{
    if (ticking) {
        ticking = false;
        
        start = SDL_GetPerformanceCounter() - start;
    }
}

void Timer::resume()
{
    if (!ticking) {
        ticking = true;

        start = SDL_GetPerformanceCounter() - start;
    }
}

void Timer::toggle()
{
    if (ticking)
        pause();
    else
        resume();
}

void Timer::reset()
{
    ticking = true;
    start = SDL_GetPerformanceCounter();
}

void Timer::handle_events(SDL_Event& e)
{
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_p)
            toggle();
        if (e.key.keysym.sym == SDLK_r)
            reset();
    }
}