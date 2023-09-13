#ifndef RENDER_H
#define RENDER_H

#include <SDL.h>

#include "balls.h"
#include "game.h"

bool render_init();
void render_close();

bool load_media();
void free_media();
void render_present();
void prepare_scene();

///////////////////////////////////

void draw_path(const std::vector<glm::vec2>& control_points, const Path& path);

#endif//RENDER_H