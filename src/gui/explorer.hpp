#ifndef REKI_EXPLORER_H
#define REKI_EXPLORER_H

#include "gui.hpp"

namespace gui {

class Explorer : public Node {
private:
    static constexpr u32 item_height = 25;
public:
    void draw(SDL_FRect bounds) override;
};

}

#endif
