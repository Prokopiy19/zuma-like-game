#ifndef PATH_H
#define PATH_H

#include <functional>
#include <vector>

#include "glm/vec2.hpp"

using Curve = std::function<glm::vec2(float)>;

struct Path {
    Path() = delete;
    Path(Curve f, float grid_h);
    glm::vec2 operator()(float t) const;
    //needs something to get t limit
    
    std::vector<glm::vec2> p;
    float h;
};

#endif//PATH_H