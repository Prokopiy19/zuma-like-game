#ifndef GAME_RECT_H
#define GAME_RECT_H

#include <SDL.h>

constexpr float GAME_WIDTH = 64.0f;
constexpr float GAME_HEIGHT = 36.0f;

extern SDL_FRect render_frect;
extern float render_ball_r;
void adjust_render_rect(int w, int h);

inline
float screen_x(float x)
{
    return render_frect.x + render_frect.w * x / GAME_WIDTH;
}
inline
float screen_y(float y)
{
    return render_frect.y + render_frect.h * y / GAME_HEIGHT;
}

inline
float scale_x(float x)
{
    return render_frect.w * x / GAME_WIDTH;
}
inline
float scale_y(float y)
{
    return render_frect.h * y / GAME_HEIGHT;
}

inline
float game_x(float x)
{
    return (x - render_frect.x) * GAME_WIDTH / render_frect.w;
}

inline
float game_y(float y)
{
    return (y - render_frect.y) * GAME_HEIGHT / render_frect.h;
}


#endif//GAME_RECT_H