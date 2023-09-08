#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

#include <cstdint>

class Timer {
public:
    float watch() const;

    void pause();
    void resume();
    void toggle();
    void reset();
    void handle_events(SDL_Event& e);

    bool running() const { return ticking; }
private:
    bool ticking = true;
    std::uint64_t start = SDL_GetPerformanceCounter();

    static float convert_to_sec(std::uint64_t t) { return static_cast<float>(t) / SDL_GetPerformanceFrequency(); };
};

#endif //TIMER_H