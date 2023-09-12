#include "balls.h"

#include <algorithm>

#include "colors.h"
#include "game.h"

namespace {

BallID id = 0;

}//namespace

void reset_balls_ids()
{
    id = 0;
}

void LineSimulation::update(float delta)
{
    spawn();

    if (ids.empty())
        return;

    speed += 0.5 * acceleration * delta;
    ts.back() += speed * delta;
    speed += 0.5 * acceleration * delta;

    speed = std::min(speed, speed_max);

    collide_forward();
}

void LineSimulation::spawn()
{
    if (state.cnt > 0 && (ts.empty() || (ts.back() > BALL_RADIUS))) {
        Color color = static_cast<Color>(state.u(state.e));
        ids.push_back(id++);
        colors.push_back(color);
        if (ts.empty() || ts.back() > 2.0f * BALL_RADIUS)
            ts.push_back(0);
        else
            ts.push_back(ts.back() - 2.0f * BALL_RADIUS);
        --state.cnt;
    }
}

void LineSimulation::collide_forward()
{
    for (int i = ts.size()-2; i >= 0; --i)
        ts[i] = std::max(ts[i], ts[i+1] + 2.0f * BALL_RADIUS);
}

void LineSimulation::collide_backward()
{
    const int size = ts.size();
    for (int i = 1; i < size; ++i)
        ts[i] = std::min(ts[i], ts[i-1] - 2.0f * BALL_RADIUS);
}
