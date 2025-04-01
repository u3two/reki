#include "explorer.hpp"

#include "gui_state.hpp"
#include "../appstate.hpp"
#include "utils.hpp"

#include "../visitors/visitors.hpp"

using namespace gui;

void Explorer::handle_event(SDL_Event &ev)
{
    (void)ev;
}

void Explorer::draw(SDL_FRect bounds)
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           Explorer::COLOR_BG.r, Explorer::COLOR_BG.b,
                           Explorer::COLOR_BG.b, Explorer::COLOR_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);

    
    if (!GUI_STATE.listing_selected_idx) {
        draw_text_fn("click to select packet", [&](float w, float h) {
            return SDL_FRect {
                bounds.x + bounds.w / 2 - w / 2,
                bounds.y + 5,
                w, h
            };
        });
    } else {
        auto &sel = APP_STATE.packet_store.at(*GUI_STATE.listing_selected_idx);
        PacketGUIExplorer visitor {};

        sel->apply(visitor);

        i32 draw_ii = 0;
        for (auto &item : visitor.items) {
            SDL_FRect HeaderRect = {
                bounds.x, bounds.y + Explorer::ITEM_HEIGHT * draw_ii,
                bounds.w, Explorer::ITEM_HEIGHT
            };

            // draw the header
            SDL_SetRenderDrawColor(GUI_STATE.renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(GUI_STATE.renderer, &HeaderRect);
            draw_text(item.title, HeaderRect.x + 5, HeaderRect.y);
            draw_ii++;

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
    }
}
