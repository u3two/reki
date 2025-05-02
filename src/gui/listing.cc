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

u32 Listing::max_items() {
    return m_bounds.h / Listing::ITEM_HEIGHT + 1;
}

void Listing::scroll_up() 
{
    if (this->m_scroll_offset > 0)
        this->m_scroll_offset--;
}

void Listing::scroll_down()
{
    if (this->m_scroll_offset < INT32_MAX && 
        this->m_scroll_offset + this->max_items() - 1 < this->item_count)
        this->m_scroll_offset++;
}

void Listing::handle_event(SDL_Event &ev)
{
    switch (ev.type) {
        /// select the element
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            auto &mouse = ev.button;

            // check if the scrollbar was clicked
            if (in_bounds(m_scrollbar_bounds, mouse.x, mouse.y)) {
                this->m_scroll_offset = std::clamp(
                    (float(mouse.y - m_scrollbar_bounds.y) / m_scrollbar_bounds.h) * this->item_count,
                    0.f, static_cast<float>(this->item_count - this->max_items() + 1));
                return; 
            }

            // .. elsewhere in the window should be interpreted as selection
            if (in_bounds(m_bounds, mouse.x, mouse.y)) {
                u32 idx = (mouse.y - this->m_bounds.y) / Listing::ITEM_HEIGHT + m_scroll_offset;
                if (idx >= this->item_count)
                    return;
                this->m_selected = this->filtered_ii[idx];
                GUI_STATE.listing_selected_idx = this->m_selected;
            }
        } break;
        case SDL_EVENT_MOUSE_WHEEL: {
            auto &wheel = ev.wheel; 
            if (in_bounds(m_bounds, wheel.mouse_x, wheel.mouse_y)) {
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

    // filter the packet store obtaining indices of packets matching the filter text
    visitors::Filter filter;
    filter.filter_text = GUI_STATE.filter_text;

    this->filtered_ii.clear();

    i32 idx = 0;
    for (auto &packet : APP_STATE.packet_store) {
        if (filter.filter_text.empty()) {
            this->filtered_ii.push_back(idx);
        } else {
            packet->apply(filter);

            if (filter.matched)
                this->filtered_ii.push_back(idx);
        }
        idx++;
    }

    // keep track of the total number of packets that we are "aware" of.
    GUI_STATE.gui_packet_count = APP_STATE.packet_store.size();

    // quick hack to reset scroll offset when necessary
    if (GUI_STATE.filter_text_changed) {
        this->m_scroll_offset = 0;
        GUI_STATE.filter_text_changed = false;
    }

    // the total number of packets in the scrollable area, including ones not currently visible
    this->item_count = this->filtered_ii.size();

    // the max number of packets that fit into the window height
    u32 max_items = this->max_items();

    visitors::ListingData listing_data;
    u32 top_idx = std::min(this->m_scroll_offset + max_items, this->item_count);
    for (u32 fi = this->m_scroll_offset; fi < top_idx; fi++) {
        auto &packet = APP_STATE.packet_store[this->filtered_ii[fi]];

        packet->apply(listing_data);
        const ListingData &lst = listing_data.get_listing();

        SDL_FRect item_rect = { 
            bounds.x, bounds.y + static_cast<float>((fi - this->m_scroll_offset) * Listing::ITEM_HEIGHT),
            bounds.w, Listing::ITEM_HEIGHT
        };

        auto [r, g, b, _] = lst.rgb;
        // brighten the selected item
        if (this->filtered_ii[fi] == this->m_selected) {
            r = std::min(r + 30, 255);
            g = std::min(g + 30, 255);
            b = std::min(b + 30, 255);
        }

        SDL_SetRenderDrawColor(GUI_STATE.renderer, r, g, b, 255);
        SDL_RenderFillRect(GUI_STATE.renderer, &item_rect);

        u64 first_arrival = APP_STATE.packet_store.at(0)->arrival_time();
        std::stringstream listing_text;
        listing_text << "T+" << std::setfill('0') << std::setw(5)
                     << packet->arrival_time() - first_arrival << " " << lst.info;

        draw_text(listing_text.str().c_str(), item_rect.x + 5, item_rect.y);
    }

    // draw the scrollbar
    if (this->item_count > max_items) {
        SDL_FRect scrollbar_outer = {
            bounds.x + bounds.w - Listing::SCROLLBAR_WIDTH, bounds.y,
            Listing::SCROLLBAR_WIDTH, bounds.h
        };

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 150, 150, 150, 100);
        SDL_RenderFillRect(GUI_STATE.renderer, &scrollbar_outer);

        this->m_scrollbar_bounds = scrollbar_outer;

        float inner_height = ((float)max_items / this->item_count) * scrollbar_outer.h;

        SDL_FRect scrollbar_inner = {
            bounds.x + bounds.w - Listing::SCROLLBAR_WIDTH, 
            bounds.y + ((float)m_scroll_offset / this->item_count) * scrollbar_outer.h,
            Listing::SCROLLBAR_WIDTH, 
            inner_height
        };

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(GUI_STATE.renderer, &scrollbar_inner);
    }
}

#if 0
void Listing::draw(SDL_FRect bounds)
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           COLOR_DEFAULT_BG.r, COLOR_DEFAULT_BG.g, COLOR_DEFAULT_BG.b,
                           COLOR_DEFAULT_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);

    this->m_bounds = bounds;
    this->displayed_packets = APP_STATE.packet_store.size();
    u32 max_items = this->max_items();

    u32 i = 0;
    visitors::ListingData visitor {};
    for (auto &p : APP_STATE.packet_store) {
        if (i == max_items + this->m_scroll_offset)
            break; // hit the last visible element, bail

        if (i < this->m_scroll_offset) {
            i++;
            continue; // not there yet, skip till we get to the first list element in view
        }

        p->apply(visitor);
        const ListingData &lst = visitor.get_listing();

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

    // draw the scrollbar
    if (this->displayed_packets > max_items) {
        SDL_FRect scrollbar_outer = {
            bounds.x + bounds.w - Listing::SCROLLBAR_WIDTH, bounds.y,
            Listing::SCROLLBAR_WIDTH, bounds.h
        };

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 150, 150, 150, 100);
        SDL_RenderFillRect(GUI_STATE.renderer, &scrollbar_outer);

        this->m_scrollbar_bounds = scrollbar_outer;

        float inner_height = ((float)max_items / this->displayed_packets) * scrollbar_outer.h;

        SDL_FRect scrollbar_inner = {
            bounds.x + bounds.w - Listing::SCROLLBAR_WIDTH, 
            bounds.y + ((float)m_scroll_offset / this->displayed_packets) * scrollbar_outer.h,
            Listing::SCROLLBAR_WIDTH, 
            inner_height
        };

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(GUI_STATE.renderer, &scrollbar_inner);
    }
}
#endif
