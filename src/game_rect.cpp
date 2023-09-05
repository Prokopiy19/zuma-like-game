#include "game_rect.h"

SDL_FRect render_frect;
float ball_r;

void adjust_render_rect(int w, int h)
{
    if (9 * w > 16 * h) {
        render_frect.w = 16.0f * h / 9.0f;
        render_frect.h = h;
        render_frect.x = (w - render_frect.w) / 2.0f;
        render_frect.y = 0;
    }
    else {
        render_frect.w = w;
        render_frect.h = 9.0f * w / 16.0f;
        render_frect.x = 0.0f;
        render_frect.y = (h - render_frect.h) / 2.0f;
    }
    ball_r = render_frect.w / 32.f;
}
