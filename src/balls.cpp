#include "balls.h"

#include <algorithm>
#include <cmath>

#include "glm/geometric.hpp"

#include "colors.h"
#include "game.h"
#include "settings.h"

namespace {

BallID id = 0;

}//namespace

void reset_balls_ids()
{
    id = 0;
}

void LineSimulation::update(const float delta)
{
    spawn();

    if (ids.empty())
        return;
    divide_segments();
    remove_unused_segments();

    collide();
    calc_pos();
    kill_balls();

    accelerate_segments(delta/2);
    move_segments(delta);
    accelerate_segments(delta/2);
}

void LineSimulation::calc_pos()
{
    const int size = ids.size();
    pos.resize(size);
    for (int i = 0; i < size; ++i)
        pos[i] = path(ts[i]);
}

void LineSimulation::spawn()
{
    if (state.cnt > 0 && (ts.empty() || (ts.back() > BALL_RADIUS))) {
        Color color = static_cast<Color>(state.u(state.e));
        ids.push_back(id++);
        alive.push_back(true);
        colors.push_back(color);
        if (ts.empty() || ts.back() > 2.0f * BALL_RADIUS) {
            ts.push_back(0);
            seg.push_back(new_seg());
        }
        else {
            ts.push_back(ts.back() - 2.0f * BALL_RADIUS);
            auto s = seg.back();
            seg.push_back(s);
        }
        --state.cnt;
    }
}

void LineSimulation::collide()
{
    for (int i = seg.size()-2; i >= 0; --i)
        if (seg[i] != seg[i+1] && std::fabs(ts[i] - ts[i+1]) < 2.0f * BALL_RADIUS) {
            auto& segment0 = get_seg(seg[i]);
            auto& segment1 = get_seg(seg[i+1]);
            float new_vel = segment0.vel + segment1.vel;
            if (colors[i] == colors[i+1]) {
                int cnt = match_colors(i, -1, false) +
                          match_colors(i+1, 1, false);
                if (cnt >= 3 || !alive[i] || !alive[i+1]) {
                    match_colors(i, -1, true);
                    match_colors(i+1, 1, true);
                    continue;
                }
            }
            remove_seg(seg[i]);
            replace_seg(i, seg[i], seg[i+1], -1);
            get_seg(seg[i+1]).vel = new_vel;
        }
}


Segment& LineSimulation::get_seg(const SEG_ID id)
{
    for (auto& s : segments)
        if (s.id == id)
            return s;
    assert(0); // invalid get_seg call
    new_seg();
    return segments[0];
}

void LineSimulation::remove_seg(const SEG_ID id)
{
    std::erase_if(segments, [id](const Segment& seg) { return seg.id == id; });
}

SEG_ID LineSimulation::new_seg(const float vel)
{
    segments.push_back({ .id=max_seg_id++, .vel=vel });
    return max_seg_id-1;
}

void LineSimulation::replace_seg(const int i, const SEG_ID from, const SEG_ID to, int step)
{
    if (step >= 0)
        step = 1;
    else
        step = -1;
    for (int j = i; 0 <= j && j < seg.size(); j += step)
        if (seg[j] == from)
            seg[j] = to;
        else
            break;
}

void LineSimulation::move_segments(const float delta)
{
    SEG_ID curr_seg = -1;
    float curr_vel = 0.0;
    for (int i = seg.size() - 1; i >= 0; --i) {
        if (seg[i] == curr_seg) {
            ts[i] = ts[i + 1] + 2.0f * BALL_RADIUS;
        }
        else {
            curr_seg = seg[i];
            curr_vel = get_seg(seg[i]).vel;
            ts[i] += curr_vel * delta;
        }
    }
}

void LineSimulation::divide_segments()
{
    for (int i = ts.size() - 2; i >= 0; --i)
        if (std::fabs(ts[i] - ts[i+1]) > 2.0f * BALL_RADIUS + EPS &&
                seg[i] == seg[i+1]) {
            replace_seg(i, seg[i], new_seg(), -1);
        }
}

void LineSimulation::kill_balls()
{
    int j = 0;
    const int size = alive.size();
    for(int i = 0; i < size; ++i)
        if (alive[i]) {
            ids[j] = ids[i];
            colors[j] = colors[i];
            ts[j] = ts[i];
            seg[j] = seg[i];
            pos[j] = pos[i];
            ++j;
        }
    ids.resize(j);
    colors.resize(j);
    ts.resize(j);
    seg.resize(j);
    pos.resize(j);
    alive.clear();
    alive.assign(j, true);
}

void LineSimulation::remove_unused_segments()
{
    cnt_segments.assign(segments.size(), 0);
    for (auto id : seg) {
        int i = &get_seg(id) - segments.data();
        ++cnt_segments[i];
    }
    int j = 0;
    const int size = segments.size();
    for (int i = 0; i < size; ++i)
        if (cnt_segments[i] > 0)
            segments[j++] = segments[i];
    segments.resize(j);
    cnt_segments.clear();
}

int LineSimulation::match_colors(const int i, int step, const bool destroy)
{
    int cnt = 0;
    const int size = colors.size();
    step = std::min(step, 1);
    step = std::max(step, -1);
    for (int j = i; 0 <= j && j < size; j += step)
        if (seg[j] == seg[i] && colors[j] == colors[i]) {
            ++cnt;
            alive[j] = alive[j] && !destroy;
        }
        else
            break;
    return cnt;
}

void LineSimulation::accelerate_segments(const float delta)
{
    if (ids.empty())
        return;
    get_seg(seg.back()).vel += BALL_ACCEL * delta;
    
    for (int i = seg.size() - 2; i >= 0; --i)
        if (seg[i] != seg[i+1])
            if(colors[i] == colors[i+1])
                get_seg(seg[i]).vel -= BACK_ACCEL * delta;
    for (auto& segment : segments) {
        float sign = 1.0f;
        if (segment.vel < 0)
            sign = -1.0f;
        if (FRICTION_ACCEL * delta < std::fabs(segment.vel))
            segment.vel -= sign * FRICTION_ACCEL * delta;
        else
            segment.vel = 0.0f;

        segment.vel = std::min(segment.vel, speed_max);
        segment.vel = std::max(segment.vel, -BACK_SPEED);
    }
}

void LineSimulation::collide_w_ball(const int i, const glm::vec2 proj_pos, const Color color)
{
    auto tangent = path.tangent(ts[i]);
    auto vec = proj_pos - pos[i];
    auto cosp = glm::dot(tangent, vec) / glm::length(tangent) / glm::length(vec);
    float vel = get_seg(seg[i]).vel;
    auto sid = new_seg(vel);
    if (cosp >= 0) {
        insert_ball(i, ts[i] + 2.0f * BALL_RADIUS - 2.0f * EPS, new_seg(), color);
        replace_seg(i-1, seg[i+1], sid, -1);
    }
    else {
        insert_ball(i+1, ts[i] - 2.0f * BALL_RADIUS + 2.0f * EPS, new_seg(), color);
        replace_seg(i+2, seg[i], sid, 1);
    }
}

void LineSimulation::insert_ball(const int i, const float t, const SEG_ID sid, const Color color)
{
    ids.insert(ids.begin() + i, id++);
    ts.insert(ts.begin() + i, t);
    colors.insert(colors.begin() + i, color);
    pos.insert(pos.begin() + i, path(t));
    alive.insert(alive.begin() + i, true);
    seg.insert(seg.begin() + i, sid);
}