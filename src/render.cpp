#include "render.h"

#include <numbers>
#include <string>
#include <vector>

#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <SDL_image.h>

#include "colors.h"
#include "settings.h"
#include "game_rect.h"
#include "window.h"

SDL_Renderer* ptr_renderer = nullptr;

namespace {
//resources
struct {
    std::vector<SDL_Texture*> destroy_list;
    std::array<SDL_Texture*, COLOR_TOTAL> colors;
    SDL_Texture* missile = nullptr;
    SDL_Texture* path = nullptr;
    SDL_Texture* exit = nullptr;
} m;

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
    m.destroy_list.push_back(ptr_texture);
	return ptr_texture;
}

bool load_media()
{
    #define X(a, b) load_texture_from_file(m.colors[a], std::string("data/ball_") + b + std::string(".png"));
        X_COLOR_TEXTURES
    #undef X

    load_texture_from_file(m.exit, "data/exit.png");
    load_texture_from_file(m.missile, "data/missile.png");

    return true;
}

void free_media()
{
    for (auto texture : m.destroy_list)
        SDL_DestroyTexture(texture);
    m.destroy_list.clear();
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

//64x36 field game coordinate system

namespace {

void draw_ball(float x, float y, Color color)
{
    SDL_FRect rect;
    rect.x = cx(x) - render_ball_r;
    rect.y = cy(y) - render_ball_r;
    rect.w = 2.0f * render_ball_r;
    rect.h = 2.0f * render_ball_r;
    SDL_RenderCopyF(ptr_renderer, m.colors[color], nullptr, &rect);
}

void draw_circle(float x, float y, float r, Color color)
{
    SDL_FRect rect;
    rect.x = cx(x) - r;
    rect.y = cy(y) - r;
    rect.w = 2.0f * r;
    rect.h = 2.0f * r;
    SDL_RenderCopyF(ptr_renderer, m.colors[color], nullptr, &rect);
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
        m.missile, 
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
    SDL_RenderCopyF(ptr_renderer, m.exit, nullptr, &frect);
}

} // namespace

void prepare_scene()
{
    adjust_render_rect(window.width, window.height);
    SDL_SetRenderDrawColor(ptr_renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);

    SDL_RenderCopyF(ptr_renderer, m.path, nullptr, &render_frect);
    for (const auto& line : state.lines)
        for (int i = 0; i < line.balls.size(); ++i)
            draw_ball(line.balls[i].pos.x, line.balls[i].pos.y, line.balls[i].color);

    for (const auto& proj : state.projectiles)
        draw_circle(proj.pos.x, proj.pos.y, sx(proj_radius[proj.type]), proj.color);

    draw_shooter();
}

void draw_path(const std::vector<glm::vec2>& control_points, const Path& path)
{
    m.path = SDL_CreateTexture(ptr_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1920, 1080);
    m.destroy_list.push_back(m.path);
    SDL_SetRenderTarget(ptr_renderer, m.path);
    adjust_render_rect(1920, 1080);
    SDL_SetRenderDrawColor(ptr_renderer, 192, 192, 192, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(ptr_renderer);
    for (auto p : path.p) {
        draw_circle(p.x, p.y, 1.5f, COLOR_BLUE);
    }
    for (auto p : control_points)
        draw_circle(p.x, p.y, 3.0f, COLOR_RED);
    draw_exit(state.lines[0]);
    SDL_SetRenderTarget(ptr_renderer, nullptr);
}