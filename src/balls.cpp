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

    if (balls.empty())
        return;

    speed += 0.5 * acceleration * delta;
    balls.back().t += speed * delta;
    speed += 0.5 * acceleration * delta;

    speed = std::min(speed, speed_max);

    collide_forward();
}

void LineSimulation::spawn()
{
    if (state.cnt > 0 && (balls.empty() || (balls[0].t - 2 * BALL_RADIUS > 0))) {
        Color color = static_cast<Color>(state.u(state.e));
        if (balls.empty())
            balls.push_back( {
                .ball_id = id++,
                .t=0,
                .color = color
            });
        else
            balls.push_back({
                .ball_id=id,
                .t=balls.back().t - 2 * BALL_RADIUS,
                .color = color
            });
        --state.cnt;
    }
}

void LineSimulation::collide_forward()
{
    for (int i = balls.size()-2; i >= 0; --i)
        balls[i].t = std::max(balls[i].t, balls[i+1].t + 2*BALL_RADIUS);
}

void LineSimulation::collide_backward()
{
    for (int i = 1; i < balls.size(); ++i)
        balls[i].t = std::min(balls[i].t, balls[i-1].t - 2*BALL_RADIUS);
}
