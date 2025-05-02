#include "explorer.hpp"

#include "gui_state.hpp"
#include "../appstate.hpp"
#include "utils.hpp"

#include "../visitors/visitors.hpp"

#include <SDL3/SDL_events.h>
#include <iomanip>
#include <math.h>

using namespace gui;

void FoldButton::click()
{
    this->m_folded = !this->m_folded;
}

void FoldButton::draw()
{ /* */ }

void Explorer::handle_event(SDL_Event &ev)
{
    switch (ev.type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            auto &mouse = ev.button;
            for (auto &button : this->m_buttons)
                button.try_click(mouse.x, mouse.y);
        } break;
    }
}

void Explorer::draw(SDL_FRect bounds)
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           Explorer::COLOR_BG.r, Explorer::COLOR_BG.b,
                           Explorer::COLOR_BG.b, Explorer::COLOR_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);

    m_bounds = bounds;
    
    if (!GUI_STATE.listing_selected_idx) {
        draw_text_fn("click to select packet", [&](float w, float h) {
            return SDL_FRect {
                bounds.x + bounds.w / 2 - w / 2,
                bounds.y + 5,
                w, h
            };
        });
    } else {
        static i32 prev_selected_idx = -1;

        auto &sel = APP_STATE.packet_store.at(*GUI_STATE.listing_selected_idx);

        visitors::ExplorerData visitor {};
        sel->apply(visitor);

        if (GUI_STATE.listing_selected_idx != prev_selected_idx) {
            // a new item was selected, create all the buttons anew
            this->m_buttons.clear();
            for (auto &_ : visitor.items) {
                this->m_buttons.push_back( 
                    FoldButton{} 
                );
            }
            prev_selected_idx = *GUI_STATE.listing_selected_idx;
        }

        u32 item_ii = 0;
        u32 draw_ii = 0;
        for (auto &item : visitor.items) {
            FoldButton &button = m_buttons.at(item_ii);

            SDL_FRect HeaderRect = {
                bounds.x, bounds.y + Explorer::ITEM_HEIGHT * draw_ii,
                bounds.w, Explorer::ITEM_HEIGHT
            };

            button.set_bounds(HeaderRect);

            std::stringstream hdr_stream;
            hdr_stream << (button.folded() ? "[SHOW] " : "[HIDE] ") << item.title;

            // draw the header
            SDL_SetRenderDrawColor(GUI_STATE.renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(GUI_STATE.renderer, &HeaderRect);
            draw_text(hdr_stream.str().c_str(), HeaderRect.x + 5, HeaderRect.y);
            draw_ii++;

            if (!button.folded()) {
                for (auto &[key, val] : item.kv) {
                    draw_text(key,
                              HeaderRect.x + 5,
                              bounds.y + Explorer::ITEM_HEIGHT * draw_ii);

                    draw_text(val.c_str(), 
                              HeaderRect.x + HeaderRect.w / 2,
                              bounds.y + Explorer::ITEM_HEIGHT * draw_ii);

                    draw_ii++;
                }
            }

            item_ii++;
        }

        // draw hexdump
        SDL_FRect HexdumpRect {
            bounds.x, bounds.y + bounds.h * 3.f/4,
            bounds.w, bounds.h / 4
        };

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 210, 210, 210, 255);
        SDL_RenderFillRect(GUI_STATE.renderer, &HexdumpRect);

        SDL_FRect HexdumpHeaderRect { HexdumpRect };
        HexdumpHeaderRect.h = Explorer::ITEM_HEIGHT;

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 90, 90, 90, 255);
        SDL_RenderFillRect(GUI_STATE.renderer, &HexdumpHeaderRect);

        draw_text("<HEXDUMP>", HexdumpHeaderRect.x + 5, HexdumpHeaderRect.y);

        constexpr u32 BYTES_PER_ROW = 16;

        std::stringstream hexdump;
        hexdump << std::uppercase << std::hex << "00 | ";

        i32 i = 0;
        for (u8 c : sel->bytes()) {
            i++;

            hexdump << std::setw(2) << std::setfill('0')
                << +c << " ";

            if (!(i % BYTES_PER_ROW) || i == sel->header_size()) {
                draw_text(hexdump.str().c_str(), 
                          HexdumpHeaderRect.x + 8, 
                          HexdumpHeaderRect.y + std::ceil((float)i / BYTES_PER_ROW) * Explorer::ITEM_HEIGHT);
                hexdump.str("");
                hexdump << std::setw(2) << std::setfill('0') << i << " | ";

                if (i == sel->header_size())
                    break;
            }
        }
    }
}
