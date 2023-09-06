#ifndef CURSOR_H
#define CURSOR_H

#include "colors.h"

void set_cursor(Color color);
bool cursor_init();
void cursor_free();

#endif//CURSOR_H