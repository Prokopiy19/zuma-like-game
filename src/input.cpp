#include "input.h"

#include <SDL.h>
#include <SDL_events.h>

#include "window.h"

void handle_events()
{
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        
        handle_window_events(e);
    }
}
