#include "gui.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "gui_state.hpp"
#include "font.hpp"

#include "../appstate.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <iostream>

using namespace gui;

gui::State gui::GUI_STATE;

void gui::init() 
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL init failure: " << SDL_GetError() << "\n";
        throw std::runtime_error("SDL_INIT");
    }

    if (!SDL_CreateWindowAndRenderer("reki", 
                                     DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, 
                                     0, &GUI_STATE.window, &GUI_STATE.renderer)) {
        std::cerr << "Couldn't create window/renderer: " << SDL_GetError() << "\n";
        throw std::runtime_error("SDL_INIT");
    }

    SDL_SetWindowResizable(GUI_STATE.window, true);
    SDL_SetRenderDrawBlendMode(GUI_STATE.renderer, SDL_BLENDMODE_BLEND);

    if (TTF_Init() == false) {
        std::cerr << "SDL_TTF init failure: " << SDL_GetError() << "\n";
        throw std::runtime_error("SDL_INIT_TTF");
    }

    GUI_STATE.font = open_font();
    if (!GUI_STATE.font) {
        std::cerr << "SDL_TTF couldn't open font: " << SDL_GetError() << "\n";
        throw std::runtime_error("SDL_INIT_TTF_OPEN");
    }

    // we are constantly accepting text input for the filter box
    SDL_StartTextInput(GUI_STATE.window);
}

static void draw_all()
{
    // I think it is ok not to take the lock here? We can afford a 1 frame delay
    // and we are only reading the value
    if (GUI_STATE.gui_packet_count < APP_STATE.packet_store.size())
        GUI_STATE.redraw = true;

    if (!GUI_STATE.redraw)
        return;

    std::lock_guard<std::mutex> lck { APP_STATE.mutex };

    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           COLOR_DEFAULT_BG.r, COLOR_DEFAULT_BG.g, COLOR_DEFAULT_BG.b,
                           COLOR_DEFAULT_BG.a);
    SDL_RenderClear(GUI_STATE.renderer);

    GUI_STATE.layout->draw(SDL_FRect {
        0, 0,
        static_cast<float>(GUI_STATE.win_w), static_cast<float>(GUI_STATE.win_h)
    });

    SDL_RenderPresent(GUI_STATE.renderer);
    GUI_STATE.redraw = false;
}

void gui::launch()
{
    if (!GUI_STATE.window || !GUI_STATE.renderer)
        throw std::runtime_error("GUI not initialized?");

    u64 frame_start;
    SDL_Event ev;
    while (!GUI_STATE.quit) {
        frame_start = SDL_GetTicks();
        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_EVENT_QUIT: {
                    GUI_STATE.quit = true;
                } break;
                case SDL_EVENT_WINDOW_EXPOSED: {
                    GUI_STATE.redraw = true;
                } break;
                case SDL_EVENT_WINDOW_RESIZED: {
                    SDL_GetWindowSize(GUI_STATE.window, &GUI_STATE.win_w, &GUI_STATE.win_h);
                    GUI_STATE.redraw = true;
                } break;
                case SDL_EVENT_MOUSE_WHEEL:
                case SDL_EVENT_TEXT_INPUT:
                case SDL_EVENT_KEY_DOWN:
                case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                    GUI_STATE.layout->handle_event(ev);
                    GUI_STATE.redraw = true;
                } break;
                default: {}
            }
        }

        draw_all();

        u64 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FPS_DELTA)
            SDL_Delay(FPS_DELTA - frame_time);
    }

    SDL_DestroyRenderer(GUI_STATE.renderer);
    SDL_DestroyWindow(GUI_STATE.window);
    SDL_Quit();
}

void ColoredPane::draw(SDL_FRect bounds) 
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           m_color.r, m_color.g, m_color.b, m_color.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);
}
