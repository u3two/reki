#include "searchbar.hpp"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_keycode.h"
#include "gui_state.hpp"
#include "utils.hpp"

namespace gui {

void SearchBar::draw(SDL_FRect bounds)
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           SearchBar::COLOR_OUTLINE.r, SearchBar::COLOR_OUTLINE.b,
                           SearchBar::COLOR_OUTLINE.b, SearchBar::COLOR_OUTLINE.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);

    constexpr i32 outline_size = 2;
    SDL_FRect inner_bounds = {
        bounds.x + outline_size, bounds.y + outline_size,
        bounds.w - outline_size * 2, bounds.h - outline_size * 2
    };

    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           SearchBar::COLOR_INNER.r, SearchBar::COLOR_INNER.b,
                           SearchBar::COLOR_INNER.b, SearchBar::COLOR_INNER.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &inner_bounds);

    auto inner_int = SDL_Rect {
        static_cast<int>(inner_bounds.x), static_cast<int>(inner_bounds.y),
        static_cast<int>(inner_bounds.w), static_cast<int>(inner_bounds.h),
    };

    SDL_SetTextInputArea(GUI_STATE.window, &inner_int, 0);

    const char *rendered_text = !GUI_STATE.filter_text.empty() ? 
                                 GUI_STATE.filter_text.c_str() : "Type to filter packets..";

    float carret_x;
    draw_text_fn(rendered_text, [&](float w, float h) {
        auto ret = SDL_FRect {
            inner_bounds.x + 5, inner_bounds.y + inner_bounds.h / 2 - h / 2,
            w, h
        };
        carret_x = ret.x + ret.w;
        return ret;
    }, SDL_Color { 100, 100, 100, 255 });

    if (!GUI_STATE.filter_text.empty()) {
        SDL_FRect carret_bounds {
            carret_x, inner_bounds.y + 2,
            2, inner_bounds.h - 4,
        };

        SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                               SearchBar::COLOR_OUTLINE.r, SearchBar::COLOR_OUTLINE.b,
                               SearchBar::COLOR_OUTLINE.b, SearchBar::COLOR_OUTLINE.a);
        SDL_RenderFillRect(GUI_STATE.renderer, &carret_bounds);
    }
}

void SearchBar::handle_event(SDL_Event& ev)
{
    switch (ev.type) {
        case SDL_EVENT_TEXT_INPUT: {
            GUI_STATE.filter_text.append(ev.text.text);
            GUI_STATE.filter_text_changed = true;
        } break;
        case SDL_EVENT_KEY_DOWN: {
            if (ev.key.key == SDLK_BACKSPACE && !GUI_STATE.filter_text.empty()) {
                GUI_STATE.filter_text.pop_back();
                GUI_STATE.filter_text_changed = true;
            }
        } break;
    }
}

}

