#include "balls.h"

#include <algorithm>
#include <cassert>

#include "glm/geometric.hpp"

#include "colors.h"
#include "game.h"
#include "settings.h"

LineSimulation::LineSimulation(const Path& path) : path(path)
{
    balls.reserve(ESTIMATED_MAX_BALLS);

    cnt_segments.reserve(ESTIMATED_MAX_SEGMENTS);
    segments.reserve(ESTIMATED_MAX_SEGMENTS);
}

void LineSimulation::update(const float delta)
{
    spawn();

    if (balls.empty())
        return;
    divide_segments();
    remove_unused_segments();

    collide();
    calc_pos();
    kill_balls();

    accelerate_segments(delta/2);
    move_segments(delta);
    accelerate_segments(delta/2);

    if (state.state == GameState::State::gameplay && !balls.empty() && balls.front().t >= path.dest) {
        state.state = GameState::State::game_over;
    }
    if (state.state == GameState::State::game_over) {
        get_seg(balls.back().sid).vel = SPAWN_SPEED;
    }
}

void LineSimulation::calc_pos()
{
    for (int i = 0; i < balls.size(); ++i)
        balls[i].pos = get_pos(i);
}

void LineSimulation::spawn()
{
    if (state.state != GameState::State::gameplay) {
        return;
    }
    if (state.cnt > 0 && (balls.empty() || (balls.back().t > BALL_RADIUS))) {
        Ball ball;
        ball.id = state.ball_id++;
        ball.color = static_cast<Color>(state.u(state.e));
        if (balls.empty() || balls.back().t > 2.0f * BALL_RADIUS) {
            float vel = (balls.empty()) ? SPAWN_SPEED : BALL_ACCEL / FRICTION;
            ball.t = 0.0f;
            ball.sid = new_seg(vel);
        }
        else {
            ball.t = balls.back().t - 2.0f * BALL_RADIUS;
            ball.sid = balls.back().sid;
        }
        balls.emplace_back(ball);
        --state.cnt;
    }
}

void LineSimulation::collide()
{
    for (int i = int(balls.size())-2; i >= 0; --i)
        if (balls[i].sid != balls[i+1].sid && std::fabs(balls[i].t - balls[i+1].t) < 2.0f * BALL_RADIUS) {
            auto& segment0 = get_seg(balls[i].sid);
            auto& segment1 = get_seg(balls[i+1].sid);
            float new_vel = segment0.vel + segment1.vel;
            if (balls[i].color == balls[i+1].color) {
                int cnt = match_colors(i, -1, false) +
                          match_colors(i+1, 1, false);
                if (cnt >= 3 || !balls[i].alive || !balls[i+1].alive) {
                    match_colors(i, -1, true);
                    match_colors(i+1, 1, true);
                    segment1.vel = new_vel;
                    continue;
                }
            }
            remove_seg(balls[i].sid);
            replace_seg(i, balls[i].sid, balls[i+1].sid, -1);
            get_seg(balls[i+1].sid).vel = new_vel;
        }
}

Segment& LineSimulation::get_seg(const SEG_ID id)
{
    for (auto& s : segments)
        if (s.id == id)
            return s;
    assert(0); // invalid get_seg call
    new_seg(0.0f);
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
    assert(step == -1 || step == 1);
    for (int j = i; 0 <= j && j < balls.size() && balls[j].sid == from; j += step) {
        balls[j].sid = to;
    }
}

void LineSimulation::move_segments(const float delta)
{
    SEG_ID curr_seg = -1;
    float curr_vel = 0.0;
    for (int i = balls.size() - 1; i >= 0; --i) {
        if (balls[i].sid == curr_seg) {
            balls[i].t = balls[i + 1].t + 2.0f * BALL_RADIUS;
        }
        else {
            curr_seg = balls[i].sid;
            curr_vel = get_seg(balls[i].sid).vel;
            balls[i].t += curr_vel * delta;
        }
    }
}

void LineSimulation::divide_segments()
{
    for (int i = balls.size() - 2; i >= 0; --i)
        if (std::fabs(balls[i].t - balls[i+1].t) > 2.0f * BALL_RADIUS + EPS &&
                balls[i].sid == balls[i+1].sid) {
            replace_seg(i, balls[i].sid, new_seg(0.0f), -1);
        }
}

void LineSimulation::kill_balls()
{
    int j = 0;
    for(int i = 0; i < balls.size(); ++i)
        if (balls[i].alive) {
            balls[j] = balls[i];
            ++j;
        }
    balls.resize(j);
}

void LineSimulation::remove_unused_segments()
{
    cnt_segments.assign(segments.size(), 0);
    for (const auto& ball : balls) {
        const int i = &get_seg(ball.sid) - segments.data();
        ++cnt_segments[i];
    }
    int j = 0;
    for (int i = 0; i < segments.size(); ++i)
        if (cnt_segments[i] > 0)
            segments[j++] = segments[i];
    segments.resize(j);
    cnt_segments.clear();
}

int LineSimulation::match_colors(const int i, const int step, const bool destroy)
{
    assert(step == -1 || step == 1);
    int cnt = 0;
    const Color color = balls[i].color;
    const SEG_ID sid = balls[i].sid;
    for (int j = i; (0 <= j && j < balls.size()) && balls[j].sid == sid && balls[j].color == color; j += step) {
        ++cnt;
        balls[j].alive = balls[j].alive && !destroy;
    }
    return cnt;
}

void LineSimulation::accelerate_segments(const float delta)
{
    if (balls.empty())
        return;
    get_seg(balls.back().sid).vel += BALL_ACCEL * delta;
    
    for (int i = int(balls.size()) - 2; i >= 0; --i)
        if (balls[i].sid != balls[i+1].sid)
            if(balls[i].color == balls[i+1].color)
                get_seg(balls[i].sid).vel -= BACK_ACCEL * delta;
    for (auto& segment : segments) {
        segment.vel -= segment.vel * FRICTION * delta;
    }
    if (balls.front().t < 0)
        get_seg(balls.back().sid).vel = SPAWN_SPEED;
}

void LineSimulation::collide_w_ball(const int i, const glm::vec2 proj_pos, const Color color)
{
    auto tangent = path.tangent(balls[i].t);
    auto vec = proj_pos - balls[i].pos;
    auto cosp = glm::dot(tangent, vec) / glm::length(tangent) / glm::length(vec);
    auto sid = new_seg(0.0f);
    if (cosp >= 0) {
        insert_ball(i, balls[i].t + 2.0f * BALL_RADIUS - 2.0f * EPS, new_seg(0.0f), color);
        replace_seg(i-1, balls[i+1].sid, sid, -1);
    }
    else {
        insert_ball(i+1, balls[i].t - 2.0f * BALL_RADIUS + 2.0f * EPS, new_seg(0.0f), color);
        replace_seg(i, balls[i].sid, sid, -1);
    }
}

void LineSimulation::insert_ball(const int i, const float t, const SEG_ID sid, const Color color)
{
    Ball ball = {
        .id = state.ball_id++,
        .t = t,
        .color = color,
        .pos = path(t),
        .alive = true,
        .sid = sid,
    };
    balls.emplace(balls.begin() + i, ball);
}