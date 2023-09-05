#include "path.h"

#include <algorithm>
#include <cmath>

#include "glm/geometric.hpp"

Path::Path(std::function<glm::vec2(float)> f, float grid_h) : h(grid_h)
{
    constexpr float EPS = 1e-6f;
    constexpr float STEP = 0.1f;

    p.push_back(f(0.));
    float t = 0.f;
    while (true) {
        float l = t, r = t;
        while (glm::distance(f(t), f(r)) < h) {
            r += STEP;
        }
        if (r <= l)
            break; // can't move forward, nothing to do
        while ((r - l) > EPS) {
            float m = 0.5f * (l + r);
            if (glm::distance(f(t), f(m)) < h)
                l = m;
            else
                r = m;
        }
        if (distance(f(t), f(r)) < 2.f * h) {
            t = r;
            p.push_back(f(t));
        }
        else {
            // we probably reached the end
            break;
        }

    }
}

glm::vec2 Path::operator()(float t) const
{
    float x = t / h;
    x = std::max(x, 0.f);
    int i = static_cast<int>(x);
    float r = x - i;
    if (i >= p.size() - 1)
        return p.back();
    else
        return (1.0f-r)*p[i] + r*p[i+1];
}