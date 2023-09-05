#include "balls.h"

#include <algorithm>

#include "colors.h"

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
    balls[0].t += speed * delta;
    speed += 0.5 * acceleration * delta;

    speed = std::min(speed, speed_max);

    collide_forward();
}

void LineSimulation::spawn()
{
    if (cnt > 0 && (balls.empty() || (balls[0].t - 2 * RADIUS > 0))) {
        Color color = static_cast<Color>(u(*ptr_e));
        if (balls.empty())
            balls.push_front( {
                .ball_id = id++,
                .t=0,
                .color = color
            });
        else
            balls.push_front({
                .ball_id=id,
                .t=balls[0].t - 2 * RADIUS,
                .color = color
            });
        --cnt;
    }
}

void LineSimulation::collide_forward()
{
    for (int i = 1; i < balls.size(); ++i)
        balls[i].t = std::max(balls[i].t, balls[i-1].t + 2*RADIUS);
}

void LineSimulation::collide_backward()
{
    for (int i = balls.size()-2; i >= 0; --i)
        balls[i].t = std::max(balls[i].t, balls[i+1].t - 2*RADIUS);
}