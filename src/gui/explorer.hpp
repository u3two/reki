#ifndef REKI_EXPLORER_H
#define REKI_EXPLORER_H

#include "gui.hpp"

#include <vector>

namespace gui {

struct ExplorerField {
    const char *title;
    /// key-value
    std::vector<std::pair<const char *, std::string>> kv;
};

class Explorer : public Node {
private:
    static constexpr u32 ITEM_HEIGHT = 25;
    static constexpr SDL_Color COLOR_BG = { 225, 225, 225, 255 };

    SDL_FRect m_bounds;

    /// stores which headers are "folded"/hidden or shown
    std::vector<bool> m_folded;
public:
    void draw(SDL_FRect bounds) override;
    void handle_event(SDL_Event &ev) override;
};

}

#endif
