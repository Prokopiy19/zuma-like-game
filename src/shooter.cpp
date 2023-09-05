#include "shooter.h"

#include <glm/geometric.hpp>

#include "game.h"
#include "game_rect.h"

void Shooter::update(float delta)
{
    if (reload_progress >= 0)
        reload_progress -= delta;
}

void Shooter::shoot(glm::vec2 target)
{
    if (reload_progress >= 0)
        return;
    constexpr float VEL = 10.0f;
    state.projectiles.push_back({
        .pos = this->pos,
        .vel = glm::normalize(target - this->pos) * VEL,
        .type = PROJ_MISSILE,
        .color = COLOR_NONE
    });
    reload_progress = reload;
}

void Shooter::handle_events(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        shoot(glm::vec2(xx(e.button.x), yy(e.button.y)));
    }
}