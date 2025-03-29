#ifndef REKI_MENUBAR_HPP
#define REKI_MENUBAR_HPP

#include "../defs.hpp"

#include <SDL3/SDL_pixels.h>

class MenuBar {
private:
    const char *text = "Capturing traffic";
    SDL_Color color = {
        34, 132, 86, 255
    };
public:
    static constexpr i32 height = 30;

    void draw();

    void set_text(const char *text) { this->text = text; }
    void set_color(SDL_Color color) { this->color = color; }
};

#endif /* REKI_MENUBAR_HPP */
