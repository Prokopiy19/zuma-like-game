#ifndef GAME_H
#define GAME_H

#include <vector>

#include "balls.h"
#include "random_color.h"
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
    int cnt = 100; // balls not spawned yet
    BallID ball_id = 0;
    
    std::vector<LineSimulation> lines;
    
    std::vector<Shooter> shooters;

    std::vector<Projectile> projectiles;

    RandomColor random_color;

    enum class State {
        init, gameplay, game_over, waiting
    } state = State::gameplay;
};
extern GameState state;

#endif//GAME_H