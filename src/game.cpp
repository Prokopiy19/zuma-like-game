#include "game.h"

#include <vector>

#include <glm/geometric.hpp>

#include "game_rect.h"
#include "settings.h"

GameState state;

GameState::GameState()
{
    projectiles.reserve(30);
}

void GameState::update(const float delta)
{
    for (auto& line : lines) {
        line.update(delta);
    }
    for (auto& shooter : shooters) {
        shooter.update(delta);
    }
    
    move_projectiles(delta);
    find_collisions();
    projectiles_gone();
    std::erase_if(projectiles, [](Projectile proj) { return proj.type == PROJ_DEAD; });
    for (auto& line : lines) {
        line.kill_balls();
    }
}

void GameState::move_projectiles(const float delta)
{
    for (auto& proj : projectiles) {
        proj.pos += proj.vel * delta;
    }
}

void GameState::find_collisions()
{
    for (auto& proj : projectiles) {
        const auto pos = proj.pos;
        if (-2.0f * BALL_RADIUS < pos.x && pos.x < GAME_WIDTH + 2.0f * BALL_RADIUS &&
            -2.0f * BALL_RADIUS < pos.y && pos.y < GAME_HEIGHT + 2.0f * BALL_RADIUS)
                for (auto& line : lines)
                    for (int i = 0; i < line.balls.size(); ++i)
                        if (glm::distance(pos, line.balls[i].pos) < proj_radius[proj.type] + BALL_RADIUS)
                            collide(proj, line, i);
    }
}

void GameState::collide(Projectile& proj, LineSimulation& line, const int i)
{
    switch(proj.type) {
        case PROJ_BALL: {
            proj.type = PROJ_DEAD;
            line.collide_w_ball(i, proj.pos, proj.color);
            break;
        }
        case PROJ_MISSILE: {
            proj.type = PROJ_DEAD;
            line.balls[i].alive = false;
            break;
        }
        case PROJ_DEAD: {
            break; // do nothing
        }
    }
}

void GameState::projectiles_gone()
{
    for (auto& proj : projectiles) {
        if (proj.pos.x < -2.0f * GAME_WIDTH ||
            proj.pos.x > 3.0f * GAME_WIDTH || 
            proj.pos.y < -GAME_HEIGHT || 
            proj.pos.y > 2.0f * GAME_HEIGHT)
        proj.type = PROJ_DEAD;
    }
}