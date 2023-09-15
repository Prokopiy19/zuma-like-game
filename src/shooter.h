#ifndef SHOOTER_H
#define SHOOTER_H

#include <SDL_events.h>

#include "glm/vec2.hpp"

#include "projectile.h"

enum ShooterMode {
    SHOOTER_BALL, SHOOTER_MISSILE, SHOOTER_TOTAL
};

struct Shooter {
    glm::vec2 pos;
    float reload = 0.8f;
    float reload_progress = 0.0f;
    int charges = 0;
    
    void update(float delta);
    void set_mode(ShooterMode mode);
    ShooterMode get_mode() const { return mode; }
    void shoot(glm::vec2 target);
    void handle_events(SDL_Event& e);
private:
    ShooterMode mode = SHOOTER_BALL;
    Color ball_color = COLOR_RED, reserve = COLOR_RED;
    void shoot_proj(glm::vec2 target, float vel, ProjectileType proj_type, Color color);
    void set_color(Color color);
    void swap_color();
    void random_color();
};

#endif//SHOOTER_H