#include "game.h"

#include "colors.h"

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
}

void GameState::move_projectiles(float delta)
{
    for (auto& proj : projectiles)
        proj.pos += proj.vel * delta;
}

void GameState::collide()
{
    
}