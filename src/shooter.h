#ifndef SHOOTER_H
#define SHOOTER_H

#include <SDL_events.h>

#include "glm/vec2.hpp"

struct Shooter {
    glm::vec2 pos;
    float reload = 0.4f;
    float reload_progress = 0.0f;
    int charges = 0;
    
    void update(float delta);
    void shoot(glm::vec2 target);
    void handle_events(SDL_Event& e);
};

#endif//SHOOTER_H