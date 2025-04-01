#ifndef REKI_MENUBAR_H
#define REKI_MENUBAR_H

#include "gui.hpp"

#include <functional>

namespace gui {

class Button {
private:
    // side effects only?
    std::function<void()> m_fn;
public:
    explicit Button(std::function<void()> fn)
    : m_fn(fn) {}

    void draw(SDL_FRect bounds, const char *label);
    void click() { this->m_fn(); }
};

class MenuBar : public Node {
private:
    static constexpr SDL_Color COLOR_BG = { 155, 155, 155, 255 };


public:
    void draw(SDL_FRect bounds) override;
    void handle_event(SDL_Event &ev) override;
};

}

#endif
