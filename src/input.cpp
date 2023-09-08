#include "input.h"

#include <SDL.h>
#include <SDL_events.h>

#include "window.h"
#include "game.h"

void handle_events()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        state.shooters[0].handle_events(e);
        handle_window_events(e);
        state.timer.handle_events(e);
    }
}
