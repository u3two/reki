/* 
* reki's GUI
* 
* The graphical interface is divided into two panes. The left one shows a scrollable list of packets captured in the
* current session (the "Listing"). The right one shows information about the currently selected packet, 
* such as its raw data hexdump and header metadata (the "Explorer").
*
*/

#include "gui.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

#include "../defs.hpp"
#include "../shared.hpp"

#include "listing.hpp"
#include "explorer.hpp"

constexpr i32 WINDOW_WIDTH = 800;
constexpr i32 WINDOW_HEIGHT = 800;

constexpr u64 FPS = 60;
constexpr u64 FPS_DELTA = 1000/FPS;

GuiState G; 

int gui_init()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL init failure: " << SDL_GetError() << "\n";
        return -1;
    }

    if (!SDL_CreateWindowAndRenderer("reki", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &G.window, &G.renderer)) {
        std::cerr << "Couldn't create window/renderer: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_SetWindowResizable(G.window, true);

    if (TTF_Init() == false) {
        std::cerr << "SDL_TTF init failure: " << SDL_GetError() << "\n";
        return -1;
    }

    G.font = TTF_OpenFont("../fonts/RobotoMono-Regular.ttf", 16);
    if (!G.font) {
        std::cerr << "SDL_TTF couldn't open font: " << SDL_GetError() << "\n";
        return -1;
    }

    SDL_GetWindowSize(G.window, &G.win_w, &G.win_h);

    if (APPMODE == AppMode::Browse) {
        G.menubar.set_text("Browse mode");
        G.menubar.set_color(SDL_Color { 132, 112, 31, 255 });
    }

    return 0;
}

void draw_all()
{
    std::lock_guard<std::mutex> lck { PACKET_STORE_MUTEX };
    if (G.listing.get_packet_count() != static_cast<i64>(PACKET_STORE.size()))
        G.redraw = true;

    if (G.redraw) {
        SDL_SetRenderDrawColor(G.renderer, 200, 200, 200, 255);
        SDL_RenderClear(G.renderer);

        G.listing.draw();
        G.explorer.draw();
        G.menubar.draw();

        SDL_RenderPresent(G.renderer);
        G.redraw = false;
    }
}

void handle_mouse_wheel(const SDL_MouseWheelEvent &ev)
{
    if(G.listing.in_bounds(ev.mouse_x, ev.mouse_y)) {
        if (ev.y > 0)
            G.listing.scroll_up();
        else if (ev.y < 0)
            G.listing.scroll_down();
    }
}

void handle_mouse_button_down(const SDL_MouseButtonEvent &ev)
{
    if(G.listing.in_bounds(ev.x, ev.y)) {
        G.listing.select(ev.x, ev.y);
    }
}

int gui_main()
{
    if (!G.window || !G.renderer)
        return -1;

    u64 frame_start;
    SDL_Event ev;
    bool quit = false;
    while (!quit) {
        frame_start = SDL_GetTicks();
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_EVENT_QUIT: {
                    quit = true;
                } break;
                case SDL_EVENT_WINDOW_EXPOSED: {
                    G.redraw = true;
                } break;
                case SDL_EVENT_WINDOW_RESIZED: {
                    SDL_GetWindowSize(G.window, &G.win_w, &G.win_h);
                    G.redraw = true;
                } break;
                case SDL_EVENT_MOUSE_WHEEL: {
                    handle_mouse_wheel(ev.wheel);
                    G.redraw = true;
                } break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    handle_mouse_button_down(ev.button);
                    G.redraw = true;
                } break;
                default: {}
            }
        }

        draw_all();

        u64 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FPS_DELTA)
            SDL_Delay(FPS_DELTA - frame_time);
    }

    SDL_DestroyRenderer(G.renderer);
    SDL_DestroyWindow(G.window);
    SDL_Quit();

    return 0;
}
