#include "projectile.h"

std::array<float, PROJ_TOTAL> proj_radius = {
    #define X(a,b) b,
        X_PROJECTILE_TYPES
    #undef X
};