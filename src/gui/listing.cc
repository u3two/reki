#include "listing.hpp"
#include "gui.hpp"
#include "menubar.hpp"
#include "util.hpp"

#include "../shared.hpp"
#include "../visitors/visitors.hpp"

#include <iostream>
#include <iomanip>

i32 Listing::max_items()
{
    return (G.win_h - MenuBar::height) / Listing::item_height + 1;
}

i32 Listing::pixel_width()
{
    return G.win_w / 2;
}

void Listing::scroll_up() 
{
    if (this->scroll_progress > 0)
        this->scroll_progress--;
}

void Listing::scroll_down() 
{
    if (this->scroll_progress < INT32_MAX && 
        this->scroll_progress + this->max_items() < this->packet_count)
        this->scroll_progress++;
}

void Listing::select(i32 mouse_x, i32 mouse_y)
{
    if (mouse_x > this->pixel_width())
        return;

    i32 idx = mouse_y / Listing::item_height + this->scroll_progress;
    if (idx >= this->packet_count)
        return;

    this->selected = idx;
}

std::optional<std::reference_wrapper<std::unique_ptr<Packet>>> 
Listing::get_selected()
{
    if (!this->selected)
        return {};

    return PACKET_STORE.at(*this->selected);
}

Bounds Listing::get_bounds()
{
    return {
        0, MenuBar::height,
        this->pixel_width(), G.win_h
    };
}

bool Listing::in_bounds(i32 mouse_x, i32 mouse_y)
{
    Bounds listing_bounds = this->get_bounds();

    return (mouse_x > listing_bounds.x && 
            mouse_x < listing_bounds.x + listing_bounds.w &&
            mouse_y > listing_bounds.y &&
            mouse_y < listing_bounds.y + listing_bounds.h);
}

void Listing::draw() 
{
    i32 max_items = this->max_items();
    Bounds bounds = this->get_bounds();

    this->packet_count = PACKET_STORE.size();

    i32 i = 0;
    PacketGUIListing visitor {};
    for (auto &p : PACKET_STORE) {
        if (i == max_items + this->scroll_progress)
            break;

        if (i < this->scroll_progress) {
            i++;
            continue;
        }

        p->apply(visitor);
        const PacketListing &lst = visitor.get_listing();

        i32 listing_number = i - this->scroll_progress;

        SDL_FRect rect = { 
            0, bounds.y + static_cast<float>(listing_number * item_height),
            static_cast<float>(bounds.w), item_height
        };
        
        auto [r, g, b] = lst.rgb;

        if (i == this->selected) {
            r = std::min(r + 30, 255);
            g = std::min(g + 30, 255);
            b = std::min(b + 30, 255);
        }

        SDL_SetRenderDrawColor(G.renderer, r, g, b, 255);
        SDL_RenderFillRect(G.renderer, &rect);

        u64 first_arrival = PACKET_STORE.at(0)->arrival_time();

        std::stringstream listing_text;
        listing_text << "T+" << std::setfill('0') << std::setw(5)
                     << p->arrival_time() - first_arrival << " " << lst.text;


        draw_text(listing_text.str().c_str(), 5, bounds.y + listing_number * item_height);
        i++;
    }

    if (this->packet_count > max_items) {
        char buff[64];
        snprintf(buff, sizeof(buff), "[%d/%d]", this->scroll_progress, this->packet_count);
        draw_text_fn(buff, [&](float w, float h) {
            return SDL_FRect {
                bounds.w - w, static_cast<float>(bounds.y + 5),
                w, h
            };
        });
    }
}
