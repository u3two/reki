#ifndef REKI_GUI_H
#define REKI_GUI_H

#include "../defs.hpp"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <memory>
#include <iostream>
#include <variant>

namespace gui {

constexpr u64 FPS = 60;
constexpr u64 FPS_DELTA = 1000/FPS;

constexpr i32 DEFAULT_WINDOW_WIDTH = 800;
constexpr i32 DEFAULT_WINDOW_HEIGHT = 600;

constexpr SDL_Color COLOR_DEFAULT_BG = { 200, 200, 200, 255 };

enum class PaneSpanType {
    Absolute,
    Ratio
};

struct PaneSpan {
private:
    using variant_t = std::variant<int, float>;
    PaneSpanType m_type;
    variant_t m_value;
public:
    explicit PaneSpan(i32 value): m_value(value) {}
    explicit PaneSpan(float value): m_value(value) {}

    PaneSpanType type() const { return this->m_type; } 
    const variant_t &value() const { return this->m_value; } 
};

class Node {
public:
    virtual ~Node() {};

    virtual void draw(SDL_FRect bounds) = 0;
    virtual void handle_event(SDL_Event &ev) = 0;
};

class HorizontalSplit : public Node {
private:
    float m_ratio;
    std::shared_ptr<Node> m_up, m_down;
public:
    explicit HorizontalSplit(float ratio, std::shared_ptr<Node> up, std::shared_ptr<Node> down)
    : m_ratio { ratio }
    , m_up { up }
    , m_down { down }
    {}

    void draw(SDL_FRect bounds) override {
        SDL_FRect upper_bounds = {
            bounds.x, bounds.y,
            bounds.w , bounds.h * m_ratio,
        };
        m_up->draw(upper_bounds);

        SDL_FRect lower_bounds = {
            bounds.x, bounds.y + upper_bounds.h,
            bounds.w, bounds.h - upper_bounds.h
        };
        m_down->draw(lower_bounds);
    }
    
    void handle_event(SDL_Event &ev) override {
        this->m_up->handle_event(ev);
        this->m_down->handle_event(ev);
    }
};


class VerticalSplit : public Node {
private:
    float m_ratio;
    std::shared_ptr<Node> m_left, m_right;
public:
    explicit VerticalSplit(float ratio, std::shared_ptr<Node> left, std::shared_ptr<Node> right)
    : m_ratio { ratio }
    , m_left { left }
    , m_right { right }
    {}

    void draw(SDL_FRect bounds)  override {
        SDL_FRect left_bounds = {
            bounds.x, bounds.y,
            bounds.w * m_ratio, bounds.h,
        };
        m_left->draw(left_bounds);

        SDL_FRect right_bounds = {
            bounds.x + left_bounds.w, bounds.y,
            bounds.w - left_bounds.w, bounds.h,
        };
        m_right->draw(right_bounds);
    }

    void handle_event(SDL_Event &ev) override {
        this->m_left->handle_event(ev);
        this->m_right->handle_event(ev);
    }
};

class ColoredPane : public Node {
private:
    SDL_Color m_color;
public:
    explicit ColoredPane(SDL_Color color)
        : m_color(color)
    {}

    void draw(SDL_FRect bounds) override;
    void handle_event([[maybe_unused]] SDL_Event &ev) override {};
};

/// initialize the graphical interface.
void init();

/// launch/open the window and start drawing.
void launch();

}

#endif
