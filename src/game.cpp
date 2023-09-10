#include "game.h"

#include <algorithm>

#include <glm/geometric.hpp>

#include "colors.h"
#include "game_rect.h"

GameState state;

void GameState::update(float delta)
{
    for (auto& line : lines) {
        line.update(delta);
    }
    for (auto& shooter : shooters)
        shooter.update(delta);
    
    move_projectiles(delta);
    find_collisions();
    projectiles_gone();
    std::erase_if(projectiles, [](Projectile proj) { return proj.type == PROJ_DEAD; });
}

void GameState::move_projectiles(float delta)
{
    for (auto& proj : projectiles)
        proj.pos += proj.vel * delta;
}

void GameState::find_collisions()
{
    for (auto& proj : projectiles)
        if (-BALL_RADIUS < proj.pos.x && proj.pos.x < GAME_WIDTH + BALL_RADIUS &&
            -BALL_RADIUS < proj.pos.y && proj.pos.y < GAME_HEIGHT + BALL_RADIUS) {
                for (auto& line : lines)
                    for (auto& ball : line.balls) // test
                        if (glm::distance(proj.pos, line.path(ball.t)) < 2.0f * BALL_RADIUS)
                            collide(proj, ball);
            }
}

void GameState::collide(Projectile& proj, Ball& ball)
{
    switch(proj.type) {
        case PROJ_BALL: {
            proj.type = PROJ_DEAD;
            break;
        }
        case PROJ_MISSILE: {
            proj.type = PROJ_DEAD;
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