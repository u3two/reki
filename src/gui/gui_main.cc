/* 
* reki's GUI
* 
* The graphical interface is divided into two panes. The left one shows a scrollable list of packets captured in the
* current session (the "Listing"). The right one shows information about the currently selected packet, 
* such as its raw data hexdump and header metadata (the "Explorer").
*
*/

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "gui.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <optional>
#include <functional>
#include <sstream>

#include "../defs.hpp"
#include "../shared.hpp"
#include "../visitors/visitors.hpp"

constexpr i32 WINDOW_WIDTH = 800;
constexpr i32 WINDOW_HEIGHT = 800;

constexpr u64 FPS = 60;
constexpr u64 FPS_DELTA = 1000/FPS;

struct Bounds {
    i32 x, y, w, h;
};

class Explorer {
private:
    static constexpr u32 item_height = 25;
public:
    void draw();
};

class Listing {
private:
    static constexpr u32 item_height = 20;
    i32 scroll_progress = 0;
    std::optional<i32> selected {};

    i32 packet_count = 0;

    i32 max_items();
    i32 pixel_width();
public:
    void draw();

    Bounds get_bounds();
    bool in_bounds(i32 mouse_ix, i32 mouse_y);

    void scroll_down();
    void scroll_up();

    void select(i32 mouse_x, i32 mouse_y);

    std::optional<std::reference_wrapper<std::unique_ptr<Packet>>> 
    get_selected();
    std::optional<i32> get_selected_id();

    i32 get_packet_count() { return this->packet_count; };
};

struct {
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int win_w = 0, win_h = 0;
    TTF_Font *font = nullptr;

    bool redraw = true;

    Listing listing;
    Explorer explorer;
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

    G.font = TTF_OpenFont("../fonts/RobotoMono-Regular.ttf", 16);
    if (!G.font) {
        std::cerr << "SDL_TTF couldn't open font: " << SDL_GetError() << "\n";
        return -1;
    }

    // just to be safe?
    SDL_GetWindowSize(G.window, &G.win_w, &G.win_h);

    return 0;
}

std::optional<SDL_Texture*> draw_text_make_texture(const char *str,
                                                   SDL_Color color = {0,0,0,0})
{
    SDL_Surface *surf = TTF_RenderText_Blended(G.font, str, 0, color);
    if (!surf) {
        std::cerr << "SDL couldn't create text surface: " << SDL_GetError() << "\n";
        return {};
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(G.renderer, surf);
    SDL_DestroySurface(surf);

    return texture;
}

int draw_text(const char *str, int x, int y)
{
    auto opt_texture = draw_text_make_texture(str);
    if (!opt_texture)
        return -1;

    SDL_Texture *texture = *opt_texture;

    SDL_FRect dst;
    SDL_GetTextureSize(texture, &dst.w, &dst.h);


    dst.x = x;
    dst.y = y;

    SDL_RenderTexture(G.renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
    return 0;
}

/// Renders text at position specified by the function fn.
/// fn accepts two arguments: the width and height of the texture.
int draw_text_fn(const char *str, 
                 const std::function<SDL_FRect(float, float)> fn,
                 SDL_Color color = {0,0,0,0})
{
    auto opt_texture = draw_text_make_texture(str, color);
    if (!opt_texture)
        return -1;

    SDL_Texture *texture = *opt_texture;

    float w, h;
    SDL_GetTextureSize(texture, &w, &h);

    SDL_FRect dst = fn(w, h);

    SDL_RenderTexture(G.renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
    return 0;
}

void Explorer::draw()
{
    Bounds listing_bounds = G.listing.get_bounds();

    SDL_FRect OuterRect = {
        static_cast<float>(listing_bounds.x + listing_bounds.w),
        0.f,
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
                5,
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
                          HeaderRect.x + 5,
                          OuterRect.y + Explorer::item_height * draw_ii);

                draw_text(val.c_str(), 
                          HeaderRect.x + HeaderRect.w / 2,
                          OuterRect.y + Explorer::item_height * draw_ii);

                draw_ii++;
            }
        }
    }
}

i32 Listing::max_items()
{
    return G.win_h / Listing::item_height + 1;
}

i32 Listing::pixel_width()
{
    return G.win_w / 2;
}

void Listing::scroll_up() 
{
    if (this->scroll_progress > 0)
        this->scroll_progress--;
}

void Listing::scroll_down() 
{
    if (this->scroll_progress < INT32_MAX && 
        this->scroll_progress + this->max_items() < this->packet_count)
        this->scroll_progress++;
}

void Listing::select(i32 mouse_x, i32 mouse_y)
{
    if (mouse_x > this->pixel_width())
        return;

    i32 idx = mouse_y / Listing::item_height + this->scroll_progress;
    if (idx >= this->packet_count)
        return;

    this->selected = idx;
}

std::optional<std::reference_wrapper<std::unique_ptr<Packet>>> 
Listing::get_selected()
{
    if (!this->selected)
        return {};

    return PACKET_STORE.at(*this->selected);
}

Bounds Listing::get_bounds()
{
    return {
        0, 0,
        this->pixel_width(), G.win_h
    };
}

bool Listing::in_bounds(i32 mouse_x, i32 mouse_y)
{
    Bounds listing_bounds = this->get_bounds();

    return (mouse_x > listing_bounds.x && 
            mouse_x < listing_bounds.x + listing_bounds.w &&
            mouse_y > listing_bounds.y &&
            mouse_y < listing_bounds.y + listing_bounds.h);
}

void Listing::draw() 
{
    i32 max_items = this->max_items();
    i32 pixel_width = this->pixel_width();

    this->packet_count = PACKET_STORE.size();

    i32 i = 0;
    PacketGUIListing visitor {};
    for (auto &p : PACKET_STORE) {
        if (i == max_items + this->scroll_progress)
            break;

        if (i < this->scroll_progress) {
            i++;
            continue;
        }

        p->apply(visitor);
        const PacketListing &lst = visitor.get_listing();

        i32 listing_number = i - this->scroll_progress;

        SDL_FRect rect = { 
            0, static_cast<float>(listing_number * item_height),
            static_cast<float>(pixel_width), item_height
        };
        
        auto [r, g, b] = lst.rgb;

        if (i == this->selected) {
            r = std::min(r + 30, 255);
            g = std::min(g + 30, 255);
            b = std::min(b + 30, 255);
        }

        SDL_SetRenderDrawColor(G.renderer, r, g, b, 255);
        SDL_RenderFillRect(G.renderer, &rect);

        u64 first_arrival = PACKET_STORE.at(0)->arrival_time();

        std::stringstream listing_text;
        listing_text << "T+" << std::setfill('0') << std::setw(5)
                     << p->arrival_time() - first_arrival << " " << lst.text;


        draw_text(listing_text.str().c_str(), 5, listing_number * item_height);
        i++;
    }

    if (this->packet_count > max_items) {
        char buff[64];
        snprintf(buff, sizeof(buff), "[%d/%d]", this->scroll_progress, this->packet_count);
        draw_text_fn(buff, [&](float w, float h) {
            return SDL_FRect {
                static_cast<float>(pixel_width) - w, 5,
                w, h
            };
        });
    }
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
