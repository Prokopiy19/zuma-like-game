#ifndef GAME_H
#define GAME_H

#include <vector>

#include "balls.h"
#include "shooter.h"
#include "timer.h"
#include "projectile.h"

struct GameState {
    void update(float delta);
    void move_projectiles(float delta);
    void find_collisions();
    void projectiles_gone();
    void collide(Projectile& proj, Ball& ball);

    Timer timer;
    
    std::vector<LineSimulation> lines;
    
    std::vector<Shooter> shooters;

    std::vector<Projectile> projectiles;
};
extern GameState state;

#endif//GAME_H