#include "listing.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_rect.h"

#include "../appstate.hpp"
#include "../visitors/visitors.hpp"

#include "gui_state.hpp"
#include "utils.hpp"

#include <iostream>
#include <iomanip>

using namespace gui;

i32 Listing::max_items()
{
    return m_bounds.h / Listing::ITEM_HEIGHT + 1;
}

bool Listing::in_bounds(i32 mouse_x, i32 mouse_y)
{
    return (mouse_x > m_bounds.x && 
            mouse_x < m_bounds.x + m_bounds.w &&
            mouse_y > m_bounds.y &&
            mouse_y < m_bounds.y + m_bounds.h);
}

void Listing::scroll_up() 
{
    if (this->m_scroll_offset > 0)
        this->m_scroll_offset--;
}

void Listing::scroll_down()
{
    if (this->m_scroll_offset < INT32_MAX && 
        this->m_scroll_offset + this->max_items() < this->m_packet_count)
        this->m_scroll_offset++;
}

void Listing::handle_event(SDL_Event &ev)
{
    switch (ev.type) {
        /// select the element
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            auto &mouse = ev.button;
            if (this->in_bounds(mouse.x, mouse.y)) {
                i32 idx = (mouse.y - this->m_bounds.y) / Listing::ITEM_HEIGHT + m_scroll_offset;
                if (idx >= m_packet_count)
                    return;
                this->m_selected = idx;
                GUI_STATE.listing_selected_idx = idx;
            }
        } break;
        case SDL_EVENT_MOUSE_WHEEL: {
            auto &wheel = ev.wheel; 
            if (this->in_bounds(wheel.mouse_x, wheel.mouse_y)) {
                if (wheel.y > 0)
                    this->scroll_up();
                else if (wheel.y < 0)
                    this->scroll_down();
            }
        } break;
    }
}

void Listing::draw(SDL_FRect bounds)
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           COLOR_DEFAULT_BG.r, COLOR_DEFAULT_BG.g, COLOR_DEFAULT_BG.b,
                           COLOR_DEFAULT_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);

    this->m_bounds = bounds;
    m_packet_count = APP_STATE.packet_store.size();

    i32 max_items = this->max_items();

    i32 i = 0;
    ListingInfoFetcher visitor {};
    for (auto &p : APP_STATE.packet_store) {
        if (i == max_items + this->m_scroll_offset)
            break; // hit the last visible element, bail

        if (i < this->m_scroll_offset) {
            i++;
            continue; // not there yet, skip till we get to the first list element in view
        }

        p->apply(visitor);
        const ListingInfo &lst = visitor.get_listing();

        i32 listing_number = i - this->m_scroll_offset;

        SDL_FRect rect = { 
            bounds.x, bounds.y + static_cast<float>(listing_number * Listing::ITEM_HEIGHT),
            bounds.w, Listing::ITEM_HEIGHT
        };
        
        auto [r, g, b, _] = lst.rgb;

        if (i == this->m_selected) {
            r = std::min(r + 30, 255);
            g = std::min(g + 30, 255);
            b = std::min(b + 30, 255);
        }

        SDL_SetRenderDrawColor(GUI_STATE.renderer, r, g, b, 255);
        SDL_RenderFillRect(GUI_STATE.renderer, &rect);

        u64 first_arrival = APP_STATE.packet_store.at(0)->arrival_time();

        std::stringstream listing_text;
        listing_text << "T+" << std::setfill('0') << std::setw(5)
                     << p->arrival_time() - first_arrival << " " << lst.info;


        draw_text(listing_text.str().c_str(), rect.x + 5, rect.y);
        i++;
    }
}
