#include "render.h"

#include <string>

#include "colors.h"
#include "game_rect.h"
#include "window.h"

SDL_Renderer* ptr_renderer = nullptr;

//resources
SDL_Texture* background = nullptr;
SDL_Texture* path_texture = nullptr;
std::array<SDL_Texture*, COLOR_TOTAL> color_textures;

bool render_init()
{
    Uint32 flags = SDL_RENDERER_ACCELERATED;
    if (!window.transparent)
        flags |= SDL_RENDERER_PRESENTVSYNC;
    ptr_renderer = SDL_CreateRenderer(window.ptr, -1, flags);
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

    #define X(a, b) load_texture_from_file(color_textures[a], std::string("data/ball_") + b + std::string(".png"));
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

//64x36 field game coordinate system
//radius=1
//
//convert x

void draw_ball(float x, float y, Color color)
{
    SDL_FRect rect;
    rect.x = cx(x) - 0.5f*render_ball_r;
    rect.y = cy(y) - 0.5f*render_ball_r;
    rect.w = render_ball_r;
    rect.h = render_ball_r;
    SDL_RenderCopyF(ptr_renderer, color_textures[color], nullptr, &rect);
}

void prepare_scene()
{
    adjust_render_rect(window.width, window.height);
    SDL_SetRenderDrawColor(ptr_renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);

    SDL_RenderCopyF(ptr_renderer, path_texture, nullptr, &render_frect);
    for (const auto& ball : state.balls) {
        draw_ball(ball.pos.x, ball.pos.y, ball.color);
    }
    for (const auto& proj : state.projectiles) {
        draw_ball(proj.pos.x, proj.pos.y, COLOR_RED);
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
    SDL_SetRenderDrawColor(ptr_renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);
    // SDL_RenderCopyF(ptr_renderer, background, nullptr, &render_frect);
    for (auto p : path.p) {
        draw_circle(p.x, p.y, 3.f, COLOR_BLUE);
        // SDL_SetRenderDrawColor(ptr_renderer, 0x00, 0x00, 0xFF, 0xFF);
        // SDL_RenderDrawPointF(ptr_renderer, cx(p.x), cy(p.y));
    }
    for (auto p : control_points)
        draw_circle(p.x, p.y, 5.f, COLOR_RED);
    SDL_SetRenderTarget(ptr_renderer, nullptr);
}