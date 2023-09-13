#ifndef GAME_H
#define GAME_H

#include <vector>

#include "balls.h"
#include "shooter.h"
#include "timer.h"
#include "projectile.h"

struct GameState {
    void update(float delta);
    void calc_pos();
    void move_projectiles(float delta);
    void find_collisions();
    void projectiles_gone();
    void collide(Projectile& proj, LineSimulation& line, int ball_id);

    Timer timer;
    int cnt = 500; // balls not spawned yet
    
    std::vector<LineSimulation> lines;
    
    std::vector<Shooter> shooters;

    std::vector<Projectile> projectiles;

    std::default_random_engine e;
    std::uniform_int_distribution<> u = std::uniform_int_distribution<>(0, COLOR_TOTAL-1);
};
extern GameState state;

#endif//GAME_H