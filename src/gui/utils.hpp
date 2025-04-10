#ifndef REKI_GUI_UTILS_H
#define REKI_GUI_UTILS_H

#include <SDL3/SDL.h>
#include <functional>

#include "../defs.hpp"

namespace gui {

/// Renders text at position specified by the function fn.
/// fn accepts two arguments: the width and height of the texture.
int draw_text_fn(const char *str, 
                 const std::function<SDL_FRect(float, float)> fn,
                 SDL_Color color = {0,0,0,0});

/// draw the text 'str' at (x,y)
int draw_text(const char *str, int x, int y);

/// check if position (x,y) is in bounds of rect
bool in_bounds(SDL_FRect bounds, i32 x, i32 y);

}

#endif
