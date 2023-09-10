#ifndef BALLS_H
#define BALLS_H

#include <random>
#include <vector>

#include "glm/vec2.hpp"

#include "colors.h"
#include "path.h"

constexpr float BALL_RADIUS = 1.0;

using BallID = int;

void reset_balls_ids();

struct Ball {
    BallID ball_id;
    float t;
    Color color;
};

class LineSimulation {
public:
    LineSimulation(Path path) : path(path) { }
    
    void update(float delta);
    
    glm::vec2 get_pos(int i) const { return path(balls[i].t); }

    std::vector<Ball> balls;
    std::vector<bool> kill;
    Path path;
private:
    constexpr static float SPEED = 2.0;
    constexpr static float FRONT_SPEED = 1.0;

    float speed = 0.;
    float speed_max = SPEED;
    constexpr static float acceleration = 0.3;
    
    void collide_forward();
    void collide_backward();
    void spawn();
};

#endif //BALLS_H