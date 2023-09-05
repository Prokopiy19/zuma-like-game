#include "spline.h"

#include <glm/vec2.hpp>

glm::vec2 Spline::operator()(float t) const
{
    constexpr float EPS = 1e-3;
    int i = static_cast<int>(t);
    if (i + 3 >= p.size()) {
        if (t + 3.f < p.size() + EPS)
            return p[p.size() - 2];
        else
            return {-10'000.f, -10'000.f};
    }
    t -= i; // t to range [0, 1]
    const glm::vec2* P = &(p[i]);
    auto ret = 0.5f * (2.f*P[1] + (-P[0] + P[2])*t + (2.f*P[0] - 5.f*P[1] + 4.f*P[2] - P[3])*t*t + (-P[0] + 3.f*P[1]- 3.f*P[2] + P[3])*t*t*t);
    return ret;
}