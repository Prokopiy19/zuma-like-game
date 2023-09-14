#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm/vec2.hpp>

#include <array>

#include "colors.h"
#include "balls.h"

#define X_PROJECTILE_TYPES \
        X(PROJ_MISSILE, 0.5) \
        X(PROJ_BALL, BALL_RADIUS)

enum ProjectileType {
    #define X(a, b) a,
        X_PROJECTILE_TYPES
    #undef X
    PROJ_TOTAL,
    PROJ_DEAD,
};

extern std::array<float, PROJ_TOTAL> proj_radius;

struct Projectile {
    glm::vec2 pos;
    glm::vec2 vel;
    ProjectileType type = PROJ_BALL;
    Color color = COLOR_RED;
};

#endif//PROJECTILE_H