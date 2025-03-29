#ifndef REKI_GUI_UTIL_H
#define REKI_GUI_UTIL_H

#include "../defs.hpp"

#include <functional>
#include <SDL3/SDL.h>

struct Bounds {
    i32 x, y, w, h;
};

int draw_text(const char *str, int x, int y);
int draw_text_fn(const char *str, 
                 const std::function<SDL_FRect(float, float)> fn,
                 SDL_Color color = {0,0,0,0});

#endif /* REKI_GUI_UTIL_H */
