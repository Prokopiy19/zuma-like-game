#ifndef COLORS_H
#define COLORS_H

#include <array>
#include "SDL_image.h"

#define X_COLOR_TEXTURES \
    X(COLOR_RED, "red") \
    X(COLOR_GREEN, "green") \
    X(COLOR_BLUE, "blue")

enum Color {
    #define X(a, b) a,
        X_COLOR_TEXTURES
    #undef X
    COLOR_TOTAL, COLOR_NONE, COLOR_DEAD
};

#endif // COLORS_H