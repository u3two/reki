#ifndef REKI_EXPLORER_H
#define REKI_EXPLORER_H

#include "gui.hpp"
#include "button.hpp"

#include <vector>

namespace gui {

class FoldButton final : public Button {
private:
    static constexpr SDL_Color COLOR_BG = { 185, 185, 185, 255 };
    bool m_folded;
public:
    FoldButton(SDL_FRect bounds)
    : Button { bounds }
    , m_folded(true)
    {}

    FoldButton()
    : FoldButton { {0,0,0,0} } 
    {};

    ~FoldButton() {};

    void draw() override;
    void click() override;

    bool folded() { return m_folded; }
};

struct ExplorerData {
    const char *title;
    /// key-value
    std::vector<std::pair<const char *, std::string>> kv;
};

class Explorer : public Node {
private:
    static constexpr u32 ITEM_HEIGHT = 25;
    static constexpr SDL_Color COLOR_BG = { 225, 225, 225, 255 };

    SDL_FRect m_bounds;

    std::vector<FoldButton> m_buttons;

    /// stores which headers are "folded"/hidden or shown
    std::vector<bool> m_folded;
public:
    void draw(SDL_FRect bounds) override;
    void handle_event(SDL_Event &ev) override;
};

}

#endif
