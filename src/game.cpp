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
        line.kill.resize(line.balls.size());
    }
    for (auto& shooter : shooters)
        shooter.update(delta);
    
    move_projectiles(delta);
    find_collisions();
    projectiles_gone();
    std::erase_if(projectiles, 
                  [](Projectile proj) { return proj.type == PROJ_DEAD; });
    kill_balls();
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
                    for (int i = 0; i < line.balls.size(); ++i)
                        if (glm::distance(proj.pos, line.get_pos(i)) < 2.0f * BALL_RADIUS) {
                            collide(proj, line, i);
                            break;
                        }
            }
}

void GameState::collide(Projectile& proj, LineSimulation& line, int i)
{
    switch(proj.type) {
        case PROJ_BALL: {
            proj.type = PROJ_DEAD;
            break;
        }
        case PROJ_MISSILE: {
            proj.type = PROJ_DEAD;
            line.kill[i] = true;
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

void GameState::kill_balls()
{
    for (auto& line : lines) {
        int j = 0;
        for (int i = 0; i < line.balls.size(); ++i)
            if (!line.kill[i])
                line.balls[j++] = line.balls[i];
        line.balls.resize(j);
        line.kill.clear();
    }
}