#include "explorer.hpp"

#include <SDL3/SDL.h>

#include "../defs.hpp"
#include "../visitors/visitors.hpp"
#include "menubar.hpp"

#include <ios>
#include <iostream>
#include <iomanip>

void Explorer::draw()
{
    Bounds listing_bounds = G.listing.get_bounds();

    SDL_FRect OuterRect = {
        static_cast<float>(listing_bounds.x + listing_bounds.w),
        MenuBar::height,
        static_cast<float>(G.win_w - listing_bounds.w),
        static_cast<float>(G.win_h),
    };

    SDL_SetRenderDrawColor(G.renderer, 185, 185, 185, 255);
    SDL_RenderFillRect(G.renderer, &OuterRect);

    auto optsel = G.listing.get_selected();
    if (!optsel) {
        draw_text_fn("click to select packet", [&](float w, float h) {
            return SDL_FRect {
                OuterRect.x + OuterRect.w / 2 - w / 2,
                OuterRect.y + 5,
                w, h
            };
        });
    } else {
        auto &sel = (*optsel).get();
        PacketGUIExplorer visitor {};

        sel->apply(visitor);

        i32 draw_ii = 0;
        for (auto &item : visitor.items) {
            SDL_FRect HeaderRect = {
                OuterRect.x, OuterRect.y + Explorer::item_height * draw_ii,
                OuterRect.w, Explorer::item_height
            };

            // draw the header
            SDL_SetRenderDrawColor(G.renderer, 120, 120, 120, 255);
            SDL_RenderFillRect(G.renderer, &HeaderRect);
            draw_text(item.title, HeaderRect.x + 5, HeaderRect.y);
            draw_ii++;

            for (auto &[key, val] : item.kv) {
                draw_text(key,
                          HeaderRect.x + 8,
                          OuterRect.y + Explorer::item_height * draw_ii);

                draw_text(val.c_str(), 
                          HeaderRect.x + HeaderRect.w / 2,
                          OuterRect.y + Explorer::item_height * draw_ii);

                draw_ii++;
            }
        }

        // draw hexdump
        SDL_FRect HexdumpRect {
            OuterRect.x, OuterRect.y + OuterRect.h * 3.f/4,
            OuterRect.w, OuterRect.h / 4
        };

        SDL_SetRenderDrawColor(G.renderer, 185, 185, 185, 255);
        SDL_RenderFillRect(G.renderer, &HexdumpRect);

        SDL_FRect HexdumpHeaderRect { HexdumpRect };
        HexdumpHeaderRect.h = Explorer::item_height;

        SDL_SetRenderDrawColor(G.renderer, 90, 90, 90, 255);
        SDL_RenderFillRect(G.renderer, &HexdumpHeaderRect);

        draw_text("<HEXDUMP>", HexdumpHeaderRect.x + 5, HexdumpHeaderRect.y);

        constexpr size_t cols = 16;
        constexpr size_t rows = 5;

        std::stringstream hexdump;
        hexdump << std::uppercase << std::hex;
        hexdump << "00 | ";

        i32 ii = 0;
        i32 ri = 0;
        for (u8 c : sel->bytes()) {
            hexdump << std::setw(2) << std::setfill('0')
                << +c << " ";
            ii++;
            if (ii == cols) {
                std::string as_str = hexdump.str();
                draw_text(as_str.c_str(), 
                          HexdumpHeaderRect.x + 8, 
                          HexdumpHeaderRect.y + (ri + 1) * Explorer::item_height);
                hexdump.str(""); // clear hexdump
                hexdump << std::setw(2) << std::setfill('0') << ri * cols << " | ";
                ii = 0;
                ri++;
                if (ri == rows)
                    break;
            } 
        }
    }
}
