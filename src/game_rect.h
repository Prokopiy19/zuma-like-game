#ifndef GAME_RECT_H
#define GAME_RECT_H

#include <SDL.h>

constexpr float GAME_WIDTH = 64.0f;
constexpr float GAME_HEIGHT = 36.0f;

extern SDL_FRect render_frect;
extern float ball_r;
void adjust_render_rect(int w, int h);

inline
float cx(float x)
{
    return render_frect.x + ball_r * x / 2.f;
}
inline
float cy(float y)
{
    return render_frect.y + ball_r * y / 2.f;
}

inline
float xx(float x)
{
    return (x - render_frect.x) * GAME_WIDTH / render_frect.w;
}

inline
float yy(float y)
{
    return (y - render_frect.y) * GAME_HEIGHT / render_frect.h;
}


#endif//GAME_RECT_H