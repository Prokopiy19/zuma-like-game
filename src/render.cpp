#include "render.h"

#include <numbers>
#include <string>
#include <vector>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <SDL_image.h>

#include "colors.h"
#include "game.h"
#include "settings.h"
#include "game_rect.h"
#include "window.h"

SDL_Renderer* ptr_renderer = nullptr;

namespace {
//resources
std::vector<SDL_Texture*> destroy_list;
std::array<SDL_Texture*, COLOR_TOTAL> color_textures;
SDL_Texture* missile_texture = nullptr;
SDL_Texture* path_texture = nullptr;
SDL_Texture* exit_texture = nullptr;

bool load_texture_from_file(SDL_Texture *&ptr_texture, const std::string& path)
{
	SDL_Surface* surface = IMG_Load(path.c_str());
	if(!surface)
	{
        SDL_Log("IMG_Load %s: %s\n", path.c_str(), IMG_GetError());
        return false;
    }
    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0x00, 0xFF));
    ptr_texture = SDL_CreateTextureFromSurface(ptr_renderer, surface);
    if(!ptr_texture)
        SDL_Log("SDL_CreateTextureFromSurface failed %s: %s\n", path.c_str(), SDL_GetError());
    SDL_FreeSurface(surface);
    destroy_list.push_back(ptr_texture);
	return ptr_texture;
}

bool load_media()
{
    #define X(a, b) load_texture_from_file(color_textures[a], std::string("data/ball_") + b + std::string(".png"));
        X_COLOR_TEXTURES
    #undef X

    load_texture_from_file(exit_texture, "data/exit.png");
    load_texture_from_file(missile_texture, "data/missile.png");

    return true;
}

void free_media()
{
    for (auto texture : destroy_list)
        SDL_DestroyTexture(texture);
    destroy_list.clear();
}

} // namespace


bool render_init()
{
    render_close();
    Uint32 flags = SDL_RENDERER_ACCELERATED;
    if (!window.transparent)
        flags |= SDL_RENDERER_PRESENTVSYNC;
    ptr_renderer = SDL_CreateRenderer(window.ptr, -1, flags);
    if (!ptr_renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return false;
    }
    return load_media();
}

void render_close()
{
    if (ptr_renderer) {
        free_media();
        SDL_DestroyRenderer(ptr_renderer);
        ptr_renderer = nullptr;
    }
}

void render_present()
{
    SDL_RenderPresent(ptr_renderer);
}

//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////

namespace {

void render_texture(SDL_Texture* texture, float x, float y, float r)
{
    const SDL_Rect rect = {
        .x = int(cx(x) - sx(r)),
        .y = int(cy(y) - sx(r)),
        .w = int(sx(2 * r)),
        .h = int(sx(2 * r)),
    };
    SDL_RenderCopy(ptr_renderer, texture, nullptr, &rect);
}

void draw_shooter()
{
    if (state.shooters.empty())
        return;
    auto& shooter = state.shooters[0];
    const float a = sx(SHOOTER_RADIUS);
    SDL_FRect frect = {
        .x = cx(shooter.pos.x) - 0.5f * a,
        .y = cx(shooter.pos.y) - 0.5f * a, 
        .w = a,
        .h = a,
    };
    int x, y;
    SDL_GetMouseState(&x, &y);
    const glm::vec2 mouse_pos(xx(x), yy(y));
    const auto direction = glm::normalize(mouse_pos - shooter.pos);
    float angle = glm::acos(direction.x);
    constexpr float pi = std::numbers::pi_v<float>;
    if (direction.y < 0)
        angle = 2.0f * pi - angle;
    const float degrees = 180.0f * angle / pi;
    SDL_RenderCopyExF(
        ptr_renderer, 
        missile_texture, 
        nullptr, 
        &frect, 
        degrees, 
        nullptr, 
        SDL_FLIP_NONE
    );
}

void draw_exit(const LineSimulation& line)
{
    SDL_FRect frect;
    auto dest = line.path(line.path.dest);
    float r = render_ball_r * 1.3f;
    frect.x = cx(dest.x) - r;
    frect.y = cy(dest.y) - r;
    frect.w = 2.0f * r;
    frect.h = 2.0f * r;
    SDL_RenderCopyF(ptr_renderer, exit_texture, nullptr, &frect);
}

} // namespace

void prepare_scene()
{
    adjust_render_rect(window.width, window.height);
    SDL_SetRenderDrawColor(ptr_renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);

    SDL_RenderCopyF(ptr_renderer, path_texture, nullptr, &render_frect);
    for (const auto& line : state.lines)
        for (int i = 0; i < line.balls.size(); ++i)
            render_texture(color_textures[line.balls[i].color], line.balls[i].pos.x, line.balls[i].pos.y, BALL_RADIUS);

    for (const auto& proj : state.projectiles)
        render_texture(color_textures[proj.color], proj.pos.x, proj.pos.y, proj_radius[proj.type]);

    draw_shooter();
}

void draw_path(const std::vector<glm::vec2>& control_points, const Path& path)
{
    path_texture = SDL_CreateTexture(ptr_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);
    destroy_list.push_back(path_texture);
    SDL_SetRenderTarget(ptr_renderer, path_texture);
    adjust_render_rect(1920, 1080);
    SDL_SetRenderDrawColor(ptr_renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);
    for (auto p : path.p) {
        render_texture(color_textures[COLOR_BLUE], p.x, p.y, 0.1f);
    }
    for (auto p : control_points)
        render_texture(color_textures[COLOR_RED], p.x, p.y, 0.2f);
    draw_exit(state.lines[0]);
    SDL_SetRenderTarget(ptr_renderer, nullptr);
}