#include "menubar.hpp"

#include "gui_state.hpp"
#include "../appstate.hpp"

#include <mutex>

using namespace gui;

void CaptureButton::click() 
{
    {
        std::lock_guard<std::mutex> lck { APP_STATE.mutex };
        if (m_label_idx == LABEL_START) {
            APP_STATE.capture.start();
        } else if (m_label_idx == LABEL_STOP) {
            APP_STATE.capture.stop();
        }
    }
    m_label_idx = (m_label_idx + 1) % (sizeof(LABELS)/sizeof(LABELS[0]));
}

void CaptureButton::draw()
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           CaptureButton::COLOR_BG.r, CaptureButton::COLOR_BG.b,
                           CaptureButton::COLOR_BG.b, CaptureButton::COLOR_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &m_bounds);

    
    draw_text_fn(CaptureButton::LABELS[m_label_idx], [&](float w, float h) {
        return SDL_FRect {
            m_bounds.x + m_bounds.w / 2 - w / 2,
            m_bounds.y + 5,
            w, h
        };
    });
}

void MenuBar::draw(SDL_FRect bounds) 
{
    SDL_SetRenderDrawColor(GUI_STATE.renderer, 
                           MenuBar::COLOR_BG.r, MenuBar::COLOR_BG.b,
                           MenuBar::COLOR_BG.b, MenuBar::COLOR_BG.a);
    SDL_RenderFillRect(GUI_STATE.renderer, &bounds);

    // draw the capture start/stop button
    SDL_FRect capture_rect = {
        bounds.x + 15, bounds.y + 5,
        100, bounds.h - 10
    };
    m_capture.set_bounds(capture_rect);
    m_capture.draw();
}

void MenuBar::handle_event(SDL_Event &ev)
{
    switch (ev.type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            auto &mouse = ev.button;
            m_capture.try_click(mouse.x, mouse.y);
        } break;
    };
}
