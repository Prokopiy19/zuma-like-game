#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <random>
#include <vector>

#include "balls.h"
#include "shooter.h"
#include "timer.h"
#include "projectile.h"

struct GameState {
    GameState();

    void update(float delta);
    void move_projectiles(float delta);
    void find_collisions();
    void projectiles_gone();
    void collide(Projectile& proj, LineSimulation& line, int ball_id);

    Timer timer;
    int cnt = 500; // balls not spawned yet
    
    std::vector<LineSimulation> lines;
    
    std::vector<Shooter> shooters;

    std::vector<Projectile> projectiles;

    std::default_random_engine e = std::default_random_engine(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<> u = std::uniform_int_distribution<>(0, COLOR_TOTAL-1);
};
extern GameState state;

#endif//GAME_H