#include <algorithm>
#include <chrono>
#include <random>
#include <vector>

#include <SDL.h>

#include "balls.h"
#include "cursor.h"
#include "debug.h"
#include "game.h"
#include "input.h"
#include "render.h"
#include "shooter.h"
#include "spline.h"
#include "window.h"

void close()
{
    cursor_free();
    free_media();
    render_close();
    window_close();
}

int main(int argv, char** args)
{
    if (!window_init() || !render_init() || !load_media()) {
        close();
        return 0;
    }
    if (!cursor_init()) {
        SDL_Log("cursor_init() failed!\n");
    }
    
    set_cursor(COLOR_RED);
    
    //64x36 field game coordinate system
    //radius=1
    std::vector<glm::vec2> control {
        {-10.0f, 5.0f},
        {-1.0f,  5.0f},
        {55.0f,  5.0f},
        {55.0f,  30.0f},
        {10.0f,  30.0f},
        {10.0f,  10.0f},
        {45.0f,  10.0f},
        {45.0f,  25.0f},
        {20.0f,  25.0f},
        {20.0f,  15.0f},
        {30.0f,  0.0f},
    };
    Spline spline(control);
    Path path(spline, 0.5f * BALL_RADIUS);

    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::default_random_engine e(seed);
    state.lines.emplace_back(500, path, e);
    Shooter shooter;
    shooter.pos = glm::vec2(40, 18);
    state.shooters.push_back(shooter);

    draw_test(control, path);
    float delta = 0.0f;
    while (running){
        state.timer.reset();
        handle_events();
        state.update(delta);

        if (!window.minimized)
            prepare_scene();
        render_present();

        delta = state.timer.watch();
        delta = std::min(delta, 1.0f / 24.0f);
        debug_title(window.ptr, delta);
    }
    close();
    return 0;
}