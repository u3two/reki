#ifndef REKI_LISTING_H
#define REKI_LISTING_H

#include "gui.hpp"
#include <optional>
#include <vector>

namespace gui {

struct ListingData {
    // TODO: info should probably be split into individual subsections 
    //       (if possible efficiently, doing one heap alloc etc.)
    std::string info;
    SDL_Color rgb;
};

class Listing: public Node {
private:
    static constexpr u32 ITEM_HEIGHT = 20;
    static constexpr u32 SCROLLBAR_WIDTH = 5;

    // we need to know current bounds in order for mouse clicks to register
    SDL_FRect m_bounds;
    SDL_FRect m_scrollbar_bounds;

    u32 m_scroll_offset;
    void scroll_down();
    void scroll_up();

    u32 item_count;
    std::optional<i32> m_selected;

    std::vector<i32> filtered_ii;

    u32 max_items();
public:
    Listing()
    : m_bounds {}, m_scroll_offset(0), item_count {}, m_selected {}
    {}

    void draw(SDL_FRect bounds) override;
    void handle_event(SDL_Event &ev) override;
};

}

#endif
