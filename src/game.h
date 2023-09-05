#ifndef GAME_H
#define GAME_H

#include <vector>

#include "balls.h"
#include "shooter.h"

enum ProjectileType {
    PROJ_MISSILE, PROJ_BALL
};

struct GameState {

    void update(float delta);
    
    std::vector<LineSimulation> lines;
    std::vector<Ball> balls;
    std::vector<BallID> destroyed;
    
    std::vector<Shooter> shooters;

    struct Projectile {
        glm::vec2 pos;
        glm::vec2 vel;
        int type;
    };
};
extern GameState state;

#endif//GAME_H