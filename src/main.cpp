#include <algorithm>
#include <vector>

#include <SDL.h>

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
    render_close();
    window_close();
}

constexpr int FPS_CAP = 121;
float MAX_DELTA = 1000.0f / FPS_CAP;
void cap_framerate()
{
    static Timer timer;
    float delta = 1000.0f * timer.watch();
    SDL_Delay(std::max(MAX_DELTA - delta, 0.0f));
    timer.reset();
}

int main(int argv, char** args)
{
    if (!window_init() || !render_init()) {
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
        {5.0f,   5.0f},
        {5.0f,   30.0f},
        {15.0f,  30.0f},
        {15.0f,  5.0f},
        {55.0f,  5.0f},
        {55.0f,  30.0f},
        {25.0f,  30.0f},
        {25.0f,  10.0f},
        {45.0f,  10.0f},
        {45.0f, 25.0f},
        {30.0f, 25.0f},
        {30.0f, 15.0f},
        {40.0f, 0.0f},
    };
    Spline spline(control);
    Path path(spline, 0.5f * BALL_RADIUS);

    state.lines.emplace_back(path);
    Shooter shooter;
    shooter.pos = glm::vec2(40, 18);
    state.shooters.push_back(shooter);

    draw_path(control, path);
    float delta = 0.0f;
    while (running){
        state.timer.reset();
        handle_events();
        state.update(delta);

        if (!window.minimized)
            prepare_scene();
        render_present();

        debug_title(window.ptr);
        cap_framerate();
        delta = state.timer.watch();
        delta = std::min(delta, 1.0f / 24.0f);
    }
    close();
    return 0;
}