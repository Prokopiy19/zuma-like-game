#ifndef COLORS_H
#define COLORS_H

#include <array>
#include "SDL_image.h"

#define X_COLOR_TEXTURES \
    X(COLOR_RED, "data/ball_red.png") \
    X(COLOR_GREEN, "data/ball_green.png") \
    X(COLOR_BLUE, "data/ball_blue.png")

enum Color {
    #define X(a, b) a,
        X_COLOR_TEXTURES
    #undef X
    COLOR_TOTAL, COLOR_NONE
};

#endif // COLORS_H