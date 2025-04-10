#include "menubar.hpp"

#include "gui_state.hpp"

using namespace gui;

void MenuBar::draw(SDL_FRect bounds) 
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           MenuBar::COLOR_BG.r, MenuBar::COLOR_BG.b,
                           MenuBar::COLOR_BG.b, MenuBar::COLOR_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);
}

void MenuBar::handle_event(SDL_Event &ev)
{

}
