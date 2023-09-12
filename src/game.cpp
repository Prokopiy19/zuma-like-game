#include "game.h"

#include <algorithm>

#include <glm/geometric.hpp>

#include "colors.h"
#include "game_rect.h"

GameState state;

void GameState::update(const float delta)
{
    for (auto& line : lines) {
        line.update(delta);
        line.alive.assign(line.ids.size(), true);
    }
    for (auto& shooter : shooters)
        shooter.update(delta);
    
    calc_pos();
    move_projectiles(delta);
    find_collisions();
    projectiles_gone();
    std::erase_if(projectiles, 
                  [](Projectile proj) { return proj.type == PROJ_DEAD; });
    kill_balls();
}

void GameState::calc_pos()
{
    for (auto& line : lines) {
        const int size = line.ids.size();
        line.pos.resize(size);
        for (int i = 0; i < size; ++i)
            line.pos[i] = line.path(line.ts[i]);
    }
}

void GameState::move_projectiles(const float delta)
{
    for (auto& proj : projectiles)
        proj.pos += proj.vel * delta;
}

void GameState::find_collisions()
{
    for (auto& proj : projectiles) {
        const auto pos = proj.pos;
        if (-2.0f * BALL_RADIUS < pos.x && pos.x < GAME_WIDTH + 2.0f * BALL_RADIUS &&
            -2.0f * BALL_RADIUS < pos.y && pos.y < GAME_HEIGHT + 2.0f * BALL_RADIUS) {
                for (auto& line : lines) {
                    const int size = line.pos.size();
                    for (int i = 0; i < size; ++i)
                        if (glm::distance(pos, line.pos[i]) < proj_radius[proj.type] + BALL_RADIUS)
                            collide(proj, line, i);
                }
            }
    }
}

void GameState::collide(Projectile& proj, LineSimulation& line, const int i)
{
    switch(proj.type) {
        case PROJ_BALL: {
            proj.type = PROJ_DEAD;
            break;
        }
        case PROJ_MISSILE: {
            proj.type = PROJ_DEAD;
            line.alive[i] = false;
            break;
        }
        case PROJ_DEAD: case PROJ_TOTAL: {
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
        const int size = line.alive.size();
        for(int i = 0; i < size; ++i)
            if (line.alive[i]) {
                line.ids[j] = line.ids[i];
                line.colors[j] = line.colors[i];
                line.ts[j] = line.ts[i];
                line.pos[j] = line.pos[i];
                ++j;
            }
        line.ids.resize(j);
        line.colors.resize(j);
        line.ts.resize(j);
        line.alive.clear();
    }
}