#ifndef BALLS_H
#define BALLS_H

#include <vector>

#include "glm/vec2.hpp"

#include "segment.h"
#include "colors.h"
#include "path.h"

using BallID = int;

class LineSimulation {
public:
    LineSimulation(const Path& path);
    
    void update(float delta);
    
    glm::vec2 get_pos(int i) const { return path(balls[i].t); }

    struct Ball {
        BallID id;
        float t = 0.0f;
        Color color;
        glm::vec2 pos;
        bool alive = true;
        SEG_ID sid;
    };
    std::vector<Ball> balls;
    void calc_pos();
    void kill_balls();
    void divide_segments();
    Path path;
    std::vector<Segment> segments;
    void collide_w_ball(int i, glm::vec2 pos, Color color);
private:    
    void collide();
    void spawn();

    std::vector<int> cnt_segments;
    SEG_ID max_seg_id = 0;
    Segment& get_seg(SEG_ID id);
    void remove_seg(SEG_ID id);
    SEG_ID new_seg(float vel);
    void replace_seg(int i, SEG_ID from, SEG_ID to, int step);
    void move_segments(float delta);
    void remove_unused_segments();
    int match_colors(int i, int step, bool destroy);
    void accelerate_segments(float delta);
    void insert_ball(int i, float t, SEG_ID sid, Color color);
};

#endif //BALLS_H