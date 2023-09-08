#ifndef GAME_H
#define GAME_H

#include <vector>

#include "balls.h"
#include "shooter.h"
#include "projectile.h"

struct GameState {

    void update(float delta);
    void move_projectiles(float delta);
    void collide();
    void projectiles_gone();
    
    std::vector<LineSimulation> lines;
    std::vector<Ball> balls;
    std::vector<BallID> destroyed;
    
    std::vector<Shooter> shooters;

    std::vector<Projectile> projectiles;
};
extern GameState state;

#endif//GAME_H