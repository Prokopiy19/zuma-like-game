#ifndef RANDOM_COLOR_H
#define RANDOM_COLOR_H

#include <chrono>
#include <random>

#include "colors.h"

class RandomColor {
public:
    RandomColor() : e(std::chrono::high_resolution_clock::now().time_since_epoch().count()), u(0, COLOR_TOTAL - 1) {}
    Color operator()() {
        return static_cast<Color>(u(e));
    }
private:
    std::default_random_engine e;
    std::uniform_int_distribution<> u;
};

#endif//RANDOM_COLOR_H