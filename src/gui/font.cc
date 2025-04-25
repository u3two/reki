#include "font.hpp"

#include <SDL3_ttf/SDL_ttf.h>

// symbols created by the linker inside res/font.o
extern char _binary_font_ttf_start[];
extern char _binary_font_ttf_end[];

namespace gui {

TTF_Font *open_font()
{
    SDL_IOStream *font_stream = SDL_IOFromConstMem((void*)_binary_font_ttf_start, 
                                                   _binary_font_ttf_end - _binary_font_ttf_start);

    return TTF_OpenFontIO(font_stream, true, 16);
}

}
