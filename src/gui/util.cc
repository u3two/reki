#include "gui.hpp"
#include "util.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cmath>
#include <cstdio>
#include <iostream>
#include <optional>
#include <functional>

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
                 SDL_Color color)
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
