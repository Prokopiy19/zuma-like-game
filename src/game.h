#ifndef GAME_H
#define GAME_H

#include <vector>

#include "balls.h"
#include "shooter.h"

enum ProjectileType {
    PROJ_MISSILE, PROJ_BALL, PROJ_DEAD
};

struct Projectile {
    glm::vec2 pos;
    glm::vec2 vel;
    ProjectileType type;
    Color color;
};

struct GameState {

    void update(float delta);
    void move_projectiles(float delta);
    void collide();
    void projectiles_gone();
    void delete_projectiles();
    
    std::vector<LineSimulation> lines;
    std::vector<Ball> balls;
    std::vector<BallID> destroyed;
    
    std::vector<Shooter> shooters;

    std::vector<Projectile> projectiles;
};
extern GameState state;

#endif//GAME_H