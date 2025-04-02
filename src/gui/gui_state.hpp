#ifndef REKI_GUI_STATE_H
#define REKI_GUI_STATE_H

#include "gui.hpp"
#include "listing.hpp"
#include "explorer.hpp"
#include "menubar.hpp"

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
        PaneSpan { 30 },
        std::make_shared<MenuBar>(),
        std::make_shared<VerticalSplit>(
            PaneSpan { 0.5f },
            std::make_shared<Explorer>(),
            std::make_shared<Listing>()
        )
    );

    /// an idx of the currently selected packet (in the listing)
    std::optional<u64> listing_selected_idx;

    /// count of currently displayed packets. If this is different than the actual count
    /// (most likely obtained from APP_STATE), we should redraw and update this.
    u32 displayed_packets;

    bool quit = false;
    bool redraw = true;
};

extern State GUI_STATE;

}

#endif
