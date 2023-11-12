#include "shooter.h"

#include <glm/geometric.hpp>

#include "cursor.h"
#include "game.h"
#include "game_rect.h"

void Shooter::update(float delta)
{
    if (reload_progress >= 0)
        reload_progress -= delta;
}

void Shooter::set_mode(ShooterMode mode)
{
    this->mode = mode;
    switch(mode) {
        case SHOOTER_BALL: {
            random_color();
            swap_color();
            random_color();
            break;
        }
        case SHOOTER_MISSILE: {
            charges = 3;
            break;
        }
        case SHOOTER_TOTAL: {
            this->mode = SHOOTER_BALL;
            SDL_Log("SHOOTER_TOTAL is invalid shooter mode\n");
            break;
        }
    }
}

void Shooter::shoot_proj(glm::vec2 target, float vel, ProjectileType proj_type, Color color)
{
    auto dir = target - this->pos;
    if (dir == glm::vec2(0.0f, 0.0f))
        target = this->pos + glm::vec2(1.0f, 0.0f);
    state.projectiles.push_back({
        .pos = this->pos,
        .vel = glm::normalize(target - this->pos) * vel,
        .type = proj_type,
        .color = color,
    });
}

void Shooter::shoot(glm::vec2 target)
{
    if (reload_progress >= 0)
        return;
    reload_progress = reload;
 
    switch (mode) {
        case SHOOTER_BALL: {
            const float vel = 60.0f;
            shoot_proj(target, vel, PROJ_BALL, ball_color);
            swap_color();
            random_color();
            break;
        }
        case SHOOTER_MISSILE: {
            const float vel = 60.0f;
            shoot_proj(target, vel, PROJ_MISSILE, COLOR_RED);
            break;
        }
        case SHOOTER_TOTAL: {
            break;
        }
    }
    if (--charges <= 0) {
        mode = SHOOTER_BALL;
        charges = 0;
    }
}

void Shooter::handle_events(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        switch (e.button.button) {
            case SDL_BUTTON_LEFT: {
                shoot(glm::vec2(game_x(e.button.x), game_y(e.button.y)));
                break;
            }
            case SDL_BUTTON_RIGHT: {
                swap_color();
                break;
            }
        }
    }
    else if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
            //CHEATS
            case SDLK_1: {
                set_mode(SHOOTER_BALL);
                break;
            }
            case SDLK_2: {
                set_mode(SHOOTER_MISSILE);
                charges = 999;
                break;
            }
        
        }
    }
}

void Shooter::set_color(Color color)
{
    ball_color = color;
    set_cursor(color);
}

void Shooter::swap_color()
{
    if (mode == SHOOTER_BALL) {
        Color tmp = ball_color;
        set_color(reserve);
        reserve = tmp;
    }
}

void Shooter::random_color()
{
    reserve = static_cast<Color>(state.u(state.e));
}