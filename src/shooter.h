#ifndef SHOOTER_H
#define SHOOTER_H

#include "glm/vec2.hpp"

struct Shooter {
    glm::vec2 pos;
    float reload = 1.0f;
    float reload_progress = 0.0f;
    int charges = 0;
    
    void update();
};

#endif//SHOOTER_H