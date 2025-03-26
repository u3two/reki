/* 
* reki's GUI
* 
* The graphical interface is divided into two panes. The top one shows a scrollable list of packets captured in the
* current session. The bottom one shows information about the currently selected packet, such as its raw data hexdump
* and header metadata.
*
*/

#include "gui.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

#include "../defs.hpp"

constexpr i32 WINDOW_WIDTH = 800;
constexpr i32 WINDOW_HEIGHT = 800;

constexpr u64 FPS = 60;
constexpr u64 FPS_DELTA = 1000/FPS;

struct {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
} G;

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

    return 0;
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
            if (ev.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(G.renderer, 100, 0, 0, 255);
        SDL_RenderClear(G.renderer);

        SDL_RenderPresent(G.renderer);

        u64 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FPS_DELTA)
            SDL_Delay(FPS_DELTA - frame_time);
    }
    
    return 0;
}
