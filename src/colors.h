#ifndef COLORS_H
#define COLORS_H

#define X_COLOR_TEXTURES \
    X(COLOR_RED, "red") \
    X(COLOR_GREEN, "green") \
    X(COLOR_BLUE, "blue") \
    X(COLOR_YELLOW, "yellow") \
    X(COLOR_PURPLE, "purple") \
    X(COLOR_WHITE, "white") \
    X(COLOR_CYAN, "cyan") \
    X(COLOR_BLACK, "black")

enum Color {
    #define X(a, b) a,
        X_COLOR_TEXTURES
    #undef X
    COLOR_TOTAL, COLOR_NONE
};

#endif // COLORS_H