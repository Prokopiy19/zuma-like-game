#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm/vec2.hpp>

#include <array>

#include "colors.h"
#include "settings.h"

#define X_PROJECTILE_TYPES \
        X(PROJ_MISSILE, 0.5) \
        X(PROJ_BALL, BALL_RADIUS)

enum ProjectileType {
    #define X(a, b) a,
        X_PROJECTILE_TYPES
    #undef X
    PROJ_DEAD,
};
#define X(a, b) +1
constexpr int PROJ_TOTAL = X_PROJECTILE_TYPES;
#undef X

extern std::array<float, PROJ_TOTAL> proj_radius;

struct Projectile {
    glm::vec2 pos;
    glm::vec2 vel;
    ProjectileType type = PROJ_BALL;
    Color color = COLOR_RED;
};

#endif//PROJECTILE_H