#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <glm/vec2.hpp>

#include "colors.h"

enum ProjectileType {
    PROJ_MISSILE, PROJ_BALL, PROJ_DEAD
};

struct Projectile {
    glm::vec2 pos;
    glm::vec2 vel;
    ProjectileType type;
    Color color;
};

#endif//PROJECTILE_H