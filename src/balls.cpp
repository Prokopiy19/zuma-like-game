#include "balls.h"

#include <algorithm>
#include <cmath>

#include "colors.h"
#include "game.h"

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

    auto& first = get_seg(seg.back());
    first.vel += 0.5 * acceleration * delta;
    first.vel = std::min(first.vel, speed_max);
    move_segments(delta);
    first.vel += 0.5 * acceleration * delta;
    first.vel = std::min(first.vel, speed_max);

    collide();
}

void LineSimulation::spawn()
{
    if (state.cnt > 0 && (ts.empty() || (ts.back() > BALL_RADIUS))) {
        Color color = static_cast<Color>(state.u(state.e));
        ids.push_back(id++);
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
            remove_seg(seg[i]);
            replace_seg(i, seg[i+1]);
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

SEG_ID LineSimulation::new_seg()
{
    segments.push_back({ .id=max_seg_id++, .vel=0.0f });
    return max_seg_id-1;
}

void LineSimulation::replace_seg(const int i, const SEG_ID id)
{
    const SEG_ID repl_seg = seg[i];
    for (int j = i; j >= 0; --j)
        if (seg[j] == repl_seg)
            seg[j] = id;
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
    constexpr float EPS = 1e-3f;
    for (int i = ts.size() - 2; i >= 0; --i)
        if (std::fabs(ts[i] - ts[i+1]) > 2.0f * BALL_RADIUS + EPS &&
                seg[i] == seg[i+1]) {
            replace_seg(i, new_seg());
        }
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