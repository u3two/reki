#ifndef REKI_GUI_STATE_H
#define REKI_GUI_STATE_H

#include "gui.hpp"
#include "listing.hpp"
#include "../packet.hpp"

#include <memory>
#include <optional>

namespace gui {

struct State {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    TTF_Font *font = nullptr;
    i32 win_w = DEFAULT_WINDOW_WIDTH; 
    i32 win_h = DEFAULT_WINDOW_HEIGHT;

    std::shared_ptr<Node> layout = std::make_shared<HorizontalSplit>(
        0.1,
        std::make_shared<ColoredPane>(SDL_Color {140, 140, 140, 255}),
        std::make_shared<VerticalSplit>(
            0.5,
            std::make_shared<Listing>(),
            std::make_shared<ColoredPane>(SDL_Color {180, 180, 180, 255})
        )
    );

    /// a reference to the currently selected packet (in the listing)
    std::optional<std::reference_wrapper<std::unique_ptr<Packet>>> listing_selected;

    bool quit = false;
    bool redraw = true;
};

extern State GUI_STATE;

}

#endif
