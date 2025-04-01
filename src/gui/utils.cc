#include "utils.hpp"

#include "gui_state.hpp"

#include <optional>
#include <iostream>

using namespace gui;

static
std::optional<SDL_Texture*> draw_text_make_texture(const char *str,
                                                   SDL_Color color = {0,0,0,0})
{
    SDL_Surface *surf = TTF_RenderText_Blended(GUI_STATE.font, str, 0, color);
    if (!surf) {
        std::cerr << "SDL couldn't create text surface: " << SDL_GetError() << "\n";
        return {};
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(GUI_STATE.renderer, surf);
    SDL_DestroySurface(surf);

    return texture;
}

int gui::draw_text(const char *str, int x, int y)
{
    auto opt_texture = draw_text_make_texture(str);
    if (!opt_texture)
        return -1;

    SDL_Texture *texture = *opt_texture;

    SDL_FRect dst;
    SDL_GetTextureSize(texture, &dst.w, &dst.h);


    dst.x = x;
    dst.y = y;

    SDL_RenderTexture(GUI_STATE.renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
    return 0;
}
/// Renders text at position specified by the function fn.
/// fn accepts two arguments: the width and height of the texture.
int gui::draw_text_fn(const char *str, 
                 const std::function<SDL_FRect(float, float)> fn,
                 SDL_Color color)
{
    auto opt_texture = draw_text_make_texture(str, color);
    if (!opt_texture)
        return -1;

    SDL_Texture *texture = *opt_texture;

    float w, h;
    SDL_GetTextureSize(texture, &w, &h);

    SDL_FRect dst = fn(w, h);

    SDL_RenderTexture(GUI_STATE.renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
    return 0;
}
