#ifndef REKI_MENUBAR_H
#define REKI_MENUBAR_H

#include "SDL3/SDL_rect.h"
#include "gui.hpp"
#include "button.hpp"

namespace gui {

class CaptureButton final : public Button {
private:
    static constexpr SDL_Color COLOR_BG = { 185, 185, 185, 255 };
    static constexpr const char *LABELS[] = { "START", "STOP" };
    enum {
        LABEL_START,
        LABEL_STOP
    };
    i32 m_label_idx;
public:
    explicit CaptureButton(SDL_FRect bounds)
    : Button { bounds }
    , m_label_idx(0)
    {}

    CaptureButton()
    : CaptureButton { {0,0,0,0} } 
    {};

    ~CaptureButton() override {};

    void draw() override;
    void click() override;
};

class MenuBar : public Node {
private:
    static constexpr SDL_Color COLOR_BG = { 155, 155, 155, 255 };
    CaptureButton m_capture {};
public:
    void draw(SDL_FRect bounds) override;
    void handle_event(SDL_Event &ev) override;
};

}

#endif
