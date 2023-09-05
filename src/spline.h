#ifndef SPLINE_H
#define SPLINE_H

#include <vector>

#include <glm/vec2.hpp>

struct Point {
    float x, y;
};

class Spline {
public:
    Spline (const std::vector<glm::vec2>& control_points) : p(control_points) {}
    glm::vec2 operator()(float t) const;
private:
    std::vector<glm::vec2> p;
};

#endif//SPLINE_H