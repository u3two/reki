#ifndef REKI_GUI_LISTING_H
#define REKI_GUI_LISTING_H

#include "../defs.hpp"
#include "../packet.hpp"

#include <optional>
#include <memory>

#include "util.hpp"

class Listing {
private:
    static constexpr u32 item_height = 20;
    i32 scroll_progress = 0;
    std::optional<i32> selected {};

    i32 packet_count = 0;

    i32 max_items();
    i32 pixel_width();
public:
    void draw();

    Bounds get_bounds();
    bool in_bounds(i32 mouse_ix, i32 mouse_y);

    void scroll_down();
    void scroll_up();

    void select(i32 mouse_x, i32 mouse_y);

    std::optional<std::reference_wrapper<std::unique_ptr<Packet>>> 
    get_selected();
    std::optional<i32> get_selected_id();

    i32 get_packet_count() { return this->packet_count; };
};

#endif /* REKI_GUI_LISTING_H */
