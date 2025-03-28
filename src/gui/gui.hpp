#ifndef REKI_GUI_H
#define REKI_GUI_H

#include <string>
#include <vector>
#include "../defs.hpp"

struct PacketListing {
    std::string text;
    std::tuple<u8, u8, u8> rgb;
};

struct ExplorerItem {
    const char *title;
    /// key-value
    std::vector<std::pair<const char *, std::string>> kv;
};

int gui_init();
int gui_main();

#endif /* REKI_GUI_H */
