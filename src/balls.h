#ifndef BALLS_H
#define BALLS_H

#include <random>
#include <vector>

#include "glm/vec2.hpp"

#include "segment.h"
#include "colors.h"
#include "settings.h"
#include "path.h"

using BallID = int;

void reset_balls_ids();

class LineSimulation {
public:
    LineSimulation(Path path) : path(path) { }
    
    void update(float delta);
    
    glm::vec2 get_pos(int i) const { return path(ts[i]); }

    // SOA
    std::vector<BallID> ids;
    std::vector<float> ts;
    std::vector<Color> colors;
    std::vector<glm::vec2> pos;
    std::vector<bool> alive;
    std::vector<SEG_ID> seg;
    // SOA

    void calc_pos();
    void kill_balls();
    void divide_segments();
    Path path;
    std::vector<Segment> segments;
    void collide_w_ball(int i, const glm::vec2 pos, Color color);
private:
    float speed = 0.0f;
    float speed_max = SPEED;
    
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