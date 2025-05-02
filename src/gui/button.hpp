#ifndef REKI_GUI_BUTTON_H
#define REKI_GUI_BUTTON_H

#include "utils.hpp"

namespace gui {

class Button {
protected:
    SDL_FRect m_bounds;
public:
    virtual ~Button() {};
    Button(SDL_FRect bounds) 
    : m_bounds(bounds) {};

    bool try_click(i32 x, i32 y) { 
        if (in_bounds(m_bounds, x, y)) {
            this->click();
            return true;
        }
        return false;
    }

    void set_bounds(SDL_FRect bounds) { m_bounds = bounds; };

    virtual void draw() = 0;
    virtual void click() = 0;
};

}

#endif
