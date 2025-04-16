#ifndef REKI_SEARCHBAR_H
#define REKI_SEARCHBAR_H

#include "gui.hpp"

namespace gui {

class SearchBar : public Node {
private:
    static constexpr SDL_Color COLOR_INNER = { 220, 220, 220, 255 };
    static constexpr SDL_Color COLOR_OUTLINE = { 80, 80, 80, 255 };
public:
    void draw(SDL_FRect bounds) override;
    void handle_event(SDL_Event &ev) override;
};

}

#endif
