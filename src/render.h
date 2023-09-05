#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>

#include "balls.h"
#include "game.h"

bool render_init();
void render_close();

struct Media {

    std::array<SDL_Texture*, COLOR_TOTAL> color_textures;
};

bool load_media();
void free_media();
void render_present();
void prepare_scene(const GameState& state);

///////////////////////////////////


void draw_test(const std::vector<glm::vec2>& control_points, const Path& path);

#endif//RENDER_H