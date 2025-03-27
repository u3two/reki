#ifndef REKI_GUI_H
#define REKI_GUI_H

#include <string>
#include "../defs.hpp"

struct PacketListing {
    std::string text;
    std::tuple<u8, u8, u8> rgb;
};

/// should be called on the main thread prior to gui_main()
int gui_init();
int gui_main();

#endif /* REKI_GUI_H */
