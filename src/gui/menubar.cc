#include "menubar.hpp"

#include "gui.hpp"
#include "util.hpp"

void MenuBar::draw() 
{
    SDL_FRect OuterRect = {
        0.f,
        0.f,
        static_cast<float>(G.win_w),
        MenuBar::height
    };

    SDL_SetRenderDrawColor(G.renderer, 
                           this->color.r, this->color.g, this->color.b, this->color.a);
    SDL_RenderFillRect(G.renderer, &OuterRect);

    draw_text_fn(this->text, [&](float w, float h) {
        return SDL_FRect {
            OuterRect.x + OuterRect.w / 2 - w / 2,
            OuterRect.y + 5,
            w, h
        };
    });
}
