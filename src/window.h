#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>

#include "colors.h"

#define WINDOW_NAME "trgame"

extern bool running;

constexpr int WIDTH = 1280;
constexpr int HEIGHT = 720;
struct Window {
    SDL_Window *ptr;
    bool borderless_fullscreen = false;
    bool minimized = false;
    bool maximized = false;
    int width = WIDTH;
    int height = HEIGHT;
    int display = 0;
    int old_width = WIDTH;
    int old_height = HEIGHT;
    int old_pos_x = 0;
    int old_pos_y = 0;
};
extern Window window;

bool window_init();
void window_close();
void handle_window_events(SDL_Event& e);

#endif //WINDOW_H