#include "game.h"

#include <algorithm>

#include "colors.h"
#include "game_rect.h"

GameState state;

void GameState::update(float delta)
{
    balls.clear();
    for (auto& line : lines) {
        line.update(delta);
        auto& line_balls = line.balls;
        for (int i = 0; i < line_balls.size(); ++i)
            balls.push_back({
                .ball_id = line_balls[i].ball_id,
                .pos = line.get_pos(i),
                .color = line_balls[i].color
            });
    }
    for (auto& shooter : shooters)
        shooter.update(delta);
    
    move_projectiles(delta);
    collide();
    projectiles_gone();
    std::erase_if(projectiles, [](Projectile proj) { return proj.type == PROJ_DEAD; });
}

void GameState::move_projectiles(float delta)
{
    for (auto& proj : projectiles)
        proj.pos += proj.vel * delta;
}

void GameState::collide()
{
    
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

void GameState::delete_projectiles()
{
    int j = 0;
    for (int i = 0; i < projectiles.size(); ++i)
        if (projectiles[i].type != PROJ_DEAD)
            projectiles[j++] = projectiles[i];
}