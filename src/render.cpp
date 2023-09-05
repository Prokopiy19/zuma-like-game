#include "render.h"

#include <string>

#include "colors.h"
#include "window.h"

SDL_Renderer* ptr_renderer = nullptr;

//resources
SDL_Texture* background = nullptr;
SDL_Texture* path_texture = nullptr;
std::array<SDL_Texture*, COLOR_TOTAL> color_textures;

bool render_init()
{
    ptr_renderer = SDL_CreateRenderer(window.ptr, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ptr_renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

void render_close()
{
    SDL_DestroyRenderer(ptr_renderer);
}

bool load_texture_from_file(SDL_Texture *&ptr_texture, const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if( surface == nullptr )
	{
        SDL_Log("IMG_Load %s: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));
    ptr_texture = SDL_CreateTextureFromSurface(ptr_renderer, surface);
    if(ptr_texture == nullptr)
    {
        SDL_Log("SDL_CreateTextureFromSurface failed %s: %s\n", path.c_str(), SDL_GetError());
    }
    SDL_FreeSurface(surface);
	return ptr_texture != nullptr;
}

bool load_media()
{
    load_texture_from_file(background, "data/back.jpg");

    #define X(a, b) load_texture_from_file(color_textures[a], b);
        X_COLOR_TEXTURES
    #undef X

    return true;
}

void free_texture(SDL_Texture* texture)
{
    if (texture)
        SDL_DestroyTexture(texture);
}

void free_media()
{
    //Free resources
    free_texture(background);
    free_texture(path_texture);
    for (int i = 0; i < COLOR_TOTAL; ++i)
        free_texture(color_textures[i]);
}

void render_present()
{
    SDL_RenderPresent(ptr_renderer);
}

//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////

SDL_FRect render_frect;
float ball_w, ball_h;

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
    ball_w = render_frect.w / 32.f;
    ball_h = render_frect.h / 18.f;
}

//64x36 field game coordinate system
//radius=1
//
//convert x
inline
float cx(float x)
{
    return render_frect.x + ball_w * x / 2.f;
}
inline
float cy(float y)
{
    return render_frect.y + ball_h * y / 2.f;
}

void draw_ball(float x, float y, Color color)
{
    SDL_FRect rect;
    rect.x = cx(x) - 0.5f*ball_w;
    rect.y = cy(y) - 0.5f*ball_h;
    rect.w = ball_w;
    rect.h = ball_h;
    SDL_RenderCopyF(ptr_renderer, color_textures[color], nullptr, &rect);
}

void prepare_scene(const GameState& state)
{
    adjust_render_rect(window.width, window.height);
    SDL_SetRenderDrawColor(ptr_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(ptr_renderer);

    SDL_RenderCopyF(ptr_renderer, path_texture, nullptr, &render_frect);
    for (const auto& ball : state.balls) {
        draw_ball(ball.pos.x, ball.pos.y, ball.color);
    }
}

void draw_circle(float x, float y, float r, Color color)
{
    SDL_FRect rect;
    rect.x = cx(x) - 0.5*r;
    rect.y = cy(y) - 0.5*r;
    rect.w = r;
    rect.h = r;
    SDL_RenderCopyF(ptr_renderer, color_textures[color], nullptr, &rect);
}

void draw_test(const std::vector<glm::vec2>& control_points, const Path& path)
{
    path_texture = SDL_CreateTexture(ptr_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);
    SDL_SetRenderTarget(ptr_renderer, path_texture);
    adjust_render_rect(1920, 1080);
    SDL_SetRenderDrawColor(ptr_renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);
    // SDL_RenderCopyF(ptr_renderer, background, nullptr, &render_frect);
    for (auto p : path.p) {
        draw_circle(p.x, p.y, 5.f, COLOR_BLUE);
        // SDL_SetRenderDrawColor(ptr_renderer, 0x00, 0x00, 0xFF, 0xFF);
        // SDL_RenderDrawPointF(ptr_renderer, cx(p.x), cy(p.y));
    }
    for (auto p : control_points)
        draw_circle(p.x, p.y, 5.f, COLOR_RED);
    SDL_SetRenderTarget(ptr_renderer, nullptr);
}