#ifndef SETTINGS_H
#define SETTINGS_H

constexpr float BALL_RADIUS = 1.0f;

constexpr float FRICTION_ACCEL = 1.0f;
constexpr float BALL_ACCEL = 1.0f + FRICTION_ACCEL;
constexpr float BACK_ACCEL = 5.0f + FRICTION_ACCEL;

constexpr static float SPEED = 3.0f;
constexpr static float BACK_SPEED = 15.0f;

#endif//SETTINGS_H