#ifndef REKI_GUI_H
#define REKI_GUI_H

#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../defs.hpp"

#include "listing.hpp"
#include "explorer.hpp"
#include "menubar.hpp"

struct PacketListing {
    std::string text;
    std::tuple<u8, u8, u8> rgb;
};

struct ExplorerItem {
    const char *title;
    /// key-value
    std::vector<std::pair<const char *, std::string>> kv;
};

int gui_init();
int gui_main();

struct GuiState {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int win_w = 0, win_h = 0;
    TTF_Font *font = nullptr;

    bool redraw = true;

    Listing listing;
    Explorer explorer;
    MenuBar menubar;
};

extern GuiState G;

#endif /* REKI_GUI_H */
