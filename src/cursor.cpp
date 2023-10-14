#include "cursor.h"

#include <SDL.h>
#include <SDL_image.h>

#include <array>
#include <string>

namespace {

std::array<SDL_Cursor*, COLOR_TOTAL> color_cursors;

SDL_Cursor* load_color_cursor(const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
	if(!surface)
	{
        SDL_Log("IMG_Load %s: %s\n", path.c_str(), IMG_GetError());
        return nullptr;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));

    auto cursor = SDL_CreateColorCursor(surface, surface->w/2, surface->h/2);
    if (!cursor) {
        SDL_Log("SDL_CreateColorCursor failed: %s\n", SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return cursor;
}

} // namespace

bool cursor_init()
{
    #define X(a, b) color_cursors[a] = load_color_cursor("data/cursor_" + std::string(b) + ".png");
        X_COLOR_TEXTURES
    #undef X
    for (auto cursor : color_cursors)
        if (!cursor)
            return false;
    return true;
}

void set_cursor(Color color)
{
    SDL_SetCursor(color_cursors[color]);
}

void cursor_free()
{
    for (auto cursor : color_cursors)
        SDL_FreeCursor(cursor);
}