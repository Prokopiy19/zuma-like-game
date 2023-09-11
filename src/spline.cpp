#include "spline.h"

#include <glm/vec2.hpp>

glm::vec2 Spline::operator()(float t) const
{
    constexpr float EPS = 1e-3;
    int i = static_cast<int>(t);
    if (i + 3 >= p.size()) {
        if (t + 3.0f < p.size() + EPS)
            return p[p.size() - 2];
        else
            return {-10'000.0f, -10'000.0f};
    }
    t -= i; // t to range [0, 1]
    const glm::vec2* P = &(p[i]);
    auto ret = 0.5f * (2.0f*P[1] + (-P[0] + P[2])*t + (2.0f*P[0] - 5.0f*P[1] + 4.0f*P[2] - P[3])*t*t + (-P[0] + 3.0f*P[1]- 3.0f*P[2] + P[3])*t*t*t);
    return ret;
}