#ifndef BALLS_H
#define BALLS_H

#include <array>
#include <deque>
#include <random>
#include <unordered_map>
#include <vector>

#include "glm/vec2.hpp"

#include "colors.h"
#include "path.h"

constexpr float RADIUS = 1.0;

using BallID = int;

void reset_balls_ids();

class LineSimulation {
public:
    LineSimulation(int count, Path path, std::default_random_engine &engine) : cnt(count), path(path), ptr_e(&engine) { }
    
    void update(float delta);
    
    glm::vec2 get_pos(int i) const { return path(balls[i].t); }

    struct LineEntry {
        BallID ball_id;
        float t;
        Color color;
    };
    std::deque<LineEntry> balls;
private:
    Path path;
    constexpr static float SPEED = 2.0;
    constexpr static float FRONT_SPEED = 1.0;

    float speed = 0.;
    float speed_max = SPEED;
    constexpr static float acceleration = 0.3;
    
    int cnt;
    std::default_random_engine* ptr_e;
    std::uniform_int_distribution<> u = std::uniform_int_distribution<>(0, COLOR_TOTAL-1);
    void collide_forward();
    void collide_backward();
    void spawn();
};

struct Ball {
    BallID ball_id;
    glm::vec2 pos;
    Color color;
};

#endif //BALLS_H